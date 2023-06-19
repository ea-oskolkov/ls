#include "FileItem.h"

#include <array>
#include <iostream>
#include <ctime>
#include <csignal>
#include <cstring>

#include <sys/stat.h>
#include <sys/dir.h>
#include <pwd.h>
#include <grp.h>

const std::string &FileItem::getAccess() const noexcept {
    return access;
}

uint64_t FileItem::getHardLinkCount() const noexcept {
    return hardLinkCount;
}

const std::string &FileItem::getUserName() const noexcept {
    return userName;
}

const std::string &FileItem::getGroupName() const noexcept {
    return groupName;
}

const std::string &FileItem::getSizeStr() const noexcept {
    return sizeStr;
}

std::string FileItem::getAccessStr(__mode_t mode) {
    using ItemAccess = std::pair<const __mode_t, char>;
    static constexpr std::array<ItemAccess, 9> accessMap = {
            ItemAccess{S_IRUSR, 'r'},
            ItemAccess{S_IWUSR, 'w'},
            ItemAccess{S_IXUSR, 'x'},

            ItemAccess{S_IRGRP, 'r'},
            ItemAccess{S_IWGRP, 'w'},
            ItemAccess{S_IXGRP, 'x'},

            ItemAccess{S_IROTH, 'r'},
            ItemAccess{S_IWOTH, 'w'},
            ItemAccess{S_IXOTH, 'x'},
    };

    std::string ret;

    if (S_ISLNK(mode))
        ret.append(1, 'l');
    else if (S_ISDIR(mode))
        ret.append(1, 'd');
    else if (S_ISBLK(mode))
        ret.append(1, 'b');
    else if (S_ISCHR(mode))
        ret.append(1, 'c');
    else if (S_ISFIFO(mode))
        ret.append(1, 'p');
    else if (S_ISSOCK(mode))
        ret.append(1, 's');
    else // is reg
        ret.append(1, '-');

    for (const auto &item: accessMap) {
        if (mode & item.first)
            ret.append(1, item.second);
        else
            ret.append(1, '-');
    }

    return ret;
}

void FileItem::fillDateTime(__time_t mtime) {

    // mtime
    struct tm *tmp = std::localtime(&mtime);
    if (tmp == nullptr)
        return;

    // Create a copy on the stack, because this is a pointer to an internal static buffer
    struct tm mtimeTm;
    memcpy(&mtimeTm, tmp, sizeof(mtimeTm));
    char buff[6];

    // Fill monthBuff
    buff[3] = '\0';
    if (strftime(buff, sizeof(buff), "%b", &mtimeTm) == 0)
        throw std::runtime_error("strftime return 0");
    dateColumn1 = buff;

    // Fill dayBuff
    buff[2] = '\0';
    if (strftime(buff, sizeof(buff), "%e", &mtimeTm) == 0)
        throw std::runtime_error("strftime return 0");
    dateColumn2 = buff;

    // Fill time or year
    const time_t currentTime = std::time(nullptr);
    const std::tm *currentTm = std::localtime(&currentTime);

    if (showYear(&mtimeTm, currentTm, mtime, currentTime)) {
        // Year
        dateColumn3 = std::to_string(mtimeTm.tm_year + 1900);
    } else {
        // Time
        buff[5] = '\0';
        if (strftime(buff, sizeof(buff), "%H:%M", &mtimeTm) == 0)
            throw std::runtime_error("strftime return 0");

        dateColumn3 = buff;
    }

}

std::string FileItem::getUsername(__uid_t uid) {
    passwd *pw = getpwuid(uid);
    if (pw == nullptr)
        return {};
    return pw->pw_name;
}

std::string FileItem::getGroupname(__gid_t _gid) {
    group *gid = getgrgid(_gid);
    if (gid == nullptr)
        return {};
    return gid->gr_name;
}

const std::string &FileItem::getLinkPath() const noexcept {
    return linkPath;
}

const std::string &FileItem::getFileName() const noexcept {
    return fileName;
}

const std::string &FileItem::getDateColumn1() const noexcept {
    return dateColumn1;
}

const std::string &FileItem::getDateColumn2() const noexcept {
    return dateColumn2;
}

const std::string &FileItem::getDateColumn3() const noexcept {
    return dateColumn3;
}

bool FileItem::showYear(
        const std::tm *inputTm,
        const std::tm *currentTm,
        __time_t inputTime,
        __time_t currentTime
) {
    const uint64_t inputYear = inputTm->tm_year;
    const uint64_t currentYear = currentTm->tm_year;

    if (inputYear > currentYear)
        return true;

    const auto diffYear = currentYear - inputYear;
    if (diffYear > 1)
        return true;

    else if (diffYear == 1) {
        const int diff = (11 % inputTm->tm_mon) + (currentTm->tm_mon + 1) + 1;
        if (diff > 6) { // half a year
            return true;
        } else if (currentTm->tm_mon == inputTm->tm_mon) {
            if (inputTm->tm_mday <= currentTm->tm_mday) {
                return true;
            }
        }
    }

    // diffYear == 0
    else {
        if (inputTime > currentTime) {
            // one hour ahead or more
            const double diffSec = std::difftime(inputTime, currentTime);
            if (diffSec >= (60.0 * 60.0))
                return true;
        }
    }

    return false;
}

FileItem::FileItem(
        struct stat *st,
        const char *fileName,
        const char *linkPath,
        FileItem::FuncSizeToStr funcSize
)
{
    if (linkPath != nullptr)
    {
        this->linkPath.append(linkPath);
    }

    if (S_ISBLK(st->st_mode) || S_ISCHR(st->st_mode)) {
        uint8_t major = st->st_rdev >> 8;
        uint8_t minor = st->st_rdev;
        sizeStr
            .append(std::to_string(major))
            .append(", ")
            .append(std::to_string(minor));
    }
    else {
        sizeStr = funcSize(st->st_size);
    }

    this->fileName = fileName;
    this->access = getAccessStr(st->st_mode);
    this->userName = getUsername(st->st_uid);
    this->groupName = getGroupname(st->st_gid);
    this->hardLinkCount = st->st_nlink;

    fillDateTime(st->st_mtime);
}
