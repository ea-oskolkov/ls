#include <iostream>
#include <algorithm>

#include <dirent.h>

#include "Parameters/Parameters.h"
#include "ProcessingFileEntries/ProcessingFileEntries.h"
#include "ProcessingFileEntries/Creator/ProcessingFileEntriesCreator.h"

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

static bool itemFilter(struct dirent *d) {
    return d->d_name[0] == '.';
}

int main(int argc, char *argv[]) {

    try {
        // Parse parameters
        Parameters &params = Parameters::getInstance();
        Parameters::Input &inputParams = params.parse(argc, argv);

        switch (inputParams.result) {
            case Parameters::ParseResult::ERROR_PARSE:
                std::cerr
                        << "This tool supports -l, -r, -h options"
                        << std::endl;
                return 2; // According to MAN original utility
            case Parameters::ParseResult::HELP:
                printUsage();
                return 0;
            default:;
        }

        const auto &processingDirEntries = ProcessingFileEntriesCreator::create(inputParams.flags);

        // Processing files
        if (!inputParams.files.empty()) {
            std::cout << std::endl;

            for (const auto &item: inputParams.files)
                processingDirEntries->process(item.second.get(), item.first);

            // Push data to stdout
            processingDirEntries->push();

            // Free resource and clear items
            processingDirEntries->clear();

            // Separate output
            std::cout << std::endl;
        }

        for (const auto &pathItem: inputParams.dirs) {

            // Output fileName
            if (inputParams.dirs.size() > 1 || !inputParams.files.empty())
                std::cout << pathItem << ": " << std::endl;

            // Open directory
            DIR *dir = opendir(pathItem);
            if (!dir)
                throw std::runtime_error("Cannot open dir");

            // Get items
            dirent *dirEntry;
            while ((dirEntry = readdir(dir)) != nullptr) {

                if (itemFilter(dirEntry))
                    continue;

                processingDirEntries->process(pathItem, dirEntry);
            }

            if (closedir(dir))
                throw std::runtime_error("'closedir' return error");

            // Push data to stdout
            processingDirEntries->push();

            // Free resource and clear items
            processingDirEntries->clear();

            // Separate directory output
            std::cout << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

