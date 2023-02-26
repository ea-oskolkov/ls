#ifndef LS_APPARGPARSER_H
#define LS_APPARGPARSER_H

#include "../FlagsMode.h"

#include <list>
#include <memory>

#include <sys/stat.h>

namespace AppArgParser
{
    enum class ParseStatus {
        SUCCESS,
        HELP,
        INVALID_ARGUMENTS
    };

    using PairFile = std::pair<const char *, std::unique_ptr<struct stat>>;
    struct ParseResult {
        ParseStatus status = ParseStatus::SUCCESS;
        FlagMode_t flags = FlagsMode::no;
        std::list<const char *> dirs{};
        std::list<PairFile> files{};
    };

    ParseResult parse(int argc, char **argv);
}

#endif //LS_APPARGPARSER_H
