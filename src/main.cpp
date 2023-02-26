#include "appArgParser/appArgParser.h"
#include "ProcessingFileEntries/ProcessingFileEntries.h"
#include "ProcessingFileEntries/Simply/ProcessingFileEntriesSimply.h"
#include "ProcessingFileEntries/Long/ProcessingFileEntriesLong.h"

#include <iostream>
#include <algorithm>

#include <dirent.h>

static void printUsage() {
    std::cout
            << "A simplified analogue of the UNIX utility ls."
            << std::endl
            << "Usage: ls [OPTION]... [FILE]..."
            << std::endl
            << "List information about the FILEs (the current directory by default)."
            << std::endl
            << "Sort entries alphabetically"
            << std::endl
            << "Supported options:"
            << std::endl
            << " -h\twith -l, print sizes like 1K 234M 2G etc."
            << std::endl
            << " -l\tuse a long listing format"
            << std::endl
            << " -r\treverse order while sorting"
            << std::endl
            << " --help\tdisplay this help and exit"
            << std::endl
            << "Exit status:"
            << std::endl
            << " 0  if OK,"
            << std::endl
            << " 1  if minor problems (e.g., cannot access subdirectory),"
            << std::endl
            << " 2  if serious trouble (e.g., cannot access command-line argument)."
            << std::endl;
}

static void printInvalidArgs() {
    std::cerr
            << "This tool supports -l, -r, -h, --help options"
            << std::endl;
}

static bool itemFilter(const struct dirent *d) {
    return d->d_name[0] == '.';
}

static std::unique_ptr<ProcessingFileEntries>
ProcessingFileEntriesCreate(const AppArgParser::ParseResult &parseResult) {

    if (parseResult.flags & FlagsMode::longView)
        return std::make_unique<ProcessingFileEntriesLong>(parseResult.flags);

    return std::make_unique<ProcessingFileEntriesSimply>(parseResult.flags);
}

int main(int argc, char *argv[]) {

    try {

        // Parse arguments
        const AppArgParser::ParseResult parseResult = AppArgParser::parse(argc, argv);

        switch (parseResult.status) {
            case AppArgParser::ParseStatus::INVALID_ARGUMENTS:
                printInvalidArgs();
                return 2; // According to MAN original utility
            case AppArgParser::ParseStatus::HELP:
                printUsage();
                return EXIT_SUCCESS;
            case AppArgParser::ParseStatus::SUCCESS:
                break;
        }

        const std::unique_ptr<ProcessingFileEntries> &processingFileEntries = ProcessingFileEntriesCreate(
                parseResult
        );

        // Processing input files
        if (!parseResult.files.empty()) {
            std::cout << std::endl;

            for (const auto &item: parseResult.files)
                processingFileEntries->process(item.second.get(), item.first);

            // Push data to stdout
            processingFileEntries->push();

            // Free resource and clear items
            processingFileEntries->clear();

            // Separate output
            std::cout << std::endl;
        }

        // Processing input directories
        for (const auto &directoryPath: parseResult.dirs) {

            // Output directory path
            if (parseResult.dirs.size() > 1 || !parseResult.files.empty())
                std::cout << directoryPath << ": " << std::endl;

            // Open directory
            DIR *const dir = opendir(directoryPath);
            if (!dir)
                throw std::runtime_error("Cannot open dir");

            // Get items
            const dirent *dirEntry;
            while ((dirEntry = readdir(dir)) != nullptr) {

                if (itemFilter(dirEntry))
                    continue;

                processingFileEntries->process(directoryPath, dirEntry);
            }

            if (closedir(dir))
                throw std::runtime_error("'closedir' return error");

            // Push data to stdout
            processingFileEntries->push();

            // Free resource and clear items
            processingFileEntries->clear();

            // Separate directory output
            std::cout << std::endl;
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
