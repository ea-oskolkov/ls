#include "ProcessingFileEntriesLong.h"
#include "../../auxiliary/auxiliary.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include <sys/stat.h>
#include <cstring>

void ProcessingFileEntriesLong::process(const char *dirPath, const struct dirent *dirEntry) {

    const std::string &fullPath = buildFullPath(dirPath, dirEntry->d_name);
    onlyFilepath = false;

    // Get info about file
    struct stat st;
    if (lstat(fullPath.c_str(), &st) != 0)
        throw std::runtime_error("lstat return error");

    char *linkPath = nullptr;
    char buffLinkPath[PATH_MAX + 1];
    if (S_ISLNK(st.st_mode)) {
        const ssize_t sizeStrLnk = readlink(fullPath.c_str(), buffLinkPath, sizeof(buffLinkPath) - sizeof(char));
        if (sizeStrLnk != -1) {
            buffLinkPath[sizeStrLnk] = '\0';
            linkPath = buffLinkPath;
        }
    }

    // Create new file item
    std::unique_ptr<FileItem> fileItem = std::make_unique<FileItem>(&st, dirEntry->d_name, linkPath, funcSizeToStr);

    // Calculate blocks count
    totalBlockCount += st.st_blocks;

    // Insert item to list
    listItem.push_back(std::move(fileItem));
}

void ProcessingFileEntriesLong::process(struct stat *st, const char *fileName) {

    char *linkPath = nullptr;
    char buffLinkPath[PATH_MAX + 1];
    if (S_ISLNK(st->st_mode)) {
        const ssize_t sizeStrLnk = readlink(fileName, buffLinkPath, sizeof(buffLinkPath) - sizeof(char));
        if (sizeStrLnk != -1) {
            buffLinkPath[sizeStrLnk] = '\0';
            linkPath = buffLinkPath;
        }
    }

    // Create new file item
    std::unique_ptr<FileItem> fileItem = std::make_unique<FileItem>(st, fileName, linkPath, funcSizeToStr);

    // Insert to list
    listItem.push_back(std::move(fileItem));
}

void ProcessingFileEntriesLong::push() {

    // Sort
    sortItems();

    if (!onlyFilepath) {

        // ProcessingFileEntries total block count
        std::cout << "total ";
        if (listItem.empty())
            std::cout << 0 << std::endl;

        // Output as in the original utility ls
        totalBlockCount *= defaultBlockSize;
        totalBlockCount /= defaultBlockSizeLsTool;

        if (flags & FlagsMode::humanSizeFormat)
            totalBlockCount *= defaultBlockSizeLsTool;

        std::cout << funcSizeToStr(totalBlockCount);
        std::cout << std::endl;
    }

    // Calculate column widths for formatted output
    fillIdentsInColumns();

    // ProcessingFileEntries items
    for (const auto &item: listItem) {
        std::cout
                << item->getAccess()
                << " "
                << std::setw(identsInColumns.hardLink)
                << item->getHardLinkCount()
                << " "
                << std::setw(identsInColumns.groupname)
                << item->getUserName()
                << " "
                << std::setw(identsInColumns.username)
                << item->getGroupName()
                << " "
                << std::setw(identsInColumns.size)
                << item->getSizeStr()
                << " "
                << std::setw(identsInColumns.date1)
                << item->getDateColumn1()
                << " "
                << std::setw(identsInColumns.date2)
                << item->getDateColumn2()
                << " "
                << std::setw(identsInColumns.date3)
                << item->getDateColumn3()
                << " "
                << item->getFileName();

        if (!item->getLinkPath().empty())
            std::cout << " -> " << item->getLinkPath();

        std::cout << std::endl;
    }
}

void ProcessingFileEntriesLong::sortItems() {
    const bool desc = flags & FlagsMode::descOrder;
    std::function<bool(const std::unique_ptr<FileItem> &d1, const std::unique_ptr<FileItem> &d2)> comparator;

    if (desc) // Descending order
        comparator = [](const std::unique_ptr<FileItem> &d1, const std::unique_ptr<FileItem> &d2) {
            return strcasecmp(d1->getFileName().c_str(), d2->getFileName().c_str()) > 0;
        };
    else
        comparator = [](const std::unique_ptr<FileItem> &d1, const std::unique_ptr<FileItem> &d2) {
            return strcasecmp(d1->getFileName().c_str(), d2->getFileName().c_str()) <= 0;
        };

    listItem.sort(comparator);
}

void ProcessingFileEntriesLong::fillIdentsInColumns() {

    for (const auto &item: listItem) {
        // Hardlink
        uint64_t tmp = item->getHardLinkCount();
        uint64_t cnt = 0;
        while (tmp) {
            tmp /= 10;
            ++cnt;
        }
        if (cnt > identsInColumns.hardLink)
            identsInColumns.hardLink = cnt;

        // Groupname
        cnt = item->getGroupName().length();
        if (cnt > identsInColumns.groupname)
            identsInColumns.groupname = cnt;

        // Username
        cnt = item->getUserName().length();
        if (cnt > identsInColumns.username)
            identsInColumns.username = cnt;

        // Size
        cnt = item->getSizeStr().length();
        if (cnt > identsInColumns.size)
            identsInColumns.size = cnt;

        // Date column 1
        cnt = item->getDateColumn1().length();
        if (cnt > identsInColumns.date1)
            identsInColumns.date1 = cnt;

        // Date column 2
        cnt = item->getDateColumn2().length();
        if (cnt > identsInColumns.date2)
            identsInColumns.date2 = cnt;

        // Date column 3
        cnt = item->getDateColumn3().length();
        if (cnt > identsInColumns.date3)
            identsInColumns.date3 = cnt;
    }

}

ProcessingFileEntriesLong::ProcessingFileEntriesLong(FlagMode_t flags) :
        onlyFilepath(true), ProcessingFileEntries(flags) {

    if (flags & FlagsMode::humanSizeFormat)
        funcSizeToStr = Auxiliary::getSizeHumanFormat;
    else
        funcSizeToStr = Auxiliary::getSizeDefaultFormat;
}

void ProcessingFileEntriesLong::clear() {
    totalBlockCount = 0;
    listItem.clear();
    memset(&identsInColumns, 0, sizeof(identsInColumns));
}

std::string ProcessingFileEntriesLong::buildFullPath(const char *dirPath, const char *fileName) {
    std::string fullPath(dirPath);

    if (fullPath.back() != '/')
        fullPath.append(1, '/');

    fullPath.append(fileName);

    return fullPath;
}

ProcessingFileEntriesLong::~ProcessingFileEntriesLong() = default;
