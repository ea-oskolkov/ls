#include "appArgParser.h"

#include <iostream>

#include <getopt.h>
#include <sys/stat.h>

namespace AppArgParser {

    ParseResult parse(int argc, char **argv) {
        ParseResult ret;
        opterr = 1;

        const char *short_options = "lhr";
        static constexpr int longHelpArg = 0;
        const struct option long_options[] = {
                {"help", 0, nullptr, longHelpArg}
        };

        int option;
        while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
            switch (option) {

                case longHelpArg:
                    ret.status = ParseStatus::HELP;
                    return ret;
                case '?': // Unknown argument
                    ret.status = ParseStatus::INVALID_ARGUMENTS;
                    return ret;

                case 'l':
                    ret.flags |= FlagsMode::longView;
                    break;
                case 'h':
                    ret.flags |= FlagsMode::humanSizeFormat;
                    break;
                case 'r':
                    ret.flags |= FlagsMode::descOrder;
                    break;
                default:break;
            }
        }

        // Processing directories and files
        for (int i = 1; i < argc; ++i) {
            if (argv[i][0] == '-')
                continue;

            std::unique_ptr<struct stat> st = std::make_unique<struct stat>();
            if (lstat(argv[i], st.get()) != 0)
                throw std::runtime_error("lstat return error");

            if (S_ISDIR(st->st_mode))
                ret.dirs.push_back(argv[i]);
            else {
                // Let's save the stat so as not to request twice
                ret.files.emplace_back(argv[i], std::move(st));
            }

        }

        // If directory/files are not specified, use the current one
        if (ret.dirs.empty() && ret.files.empty())
            ret.dirs.push_back("./");

        return ret;
    }

}
