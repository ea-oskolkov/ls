#include <iostream>

#include <getopt.h>
#include <sys/stat.h>

#include "Parameters.h"

Parameters::Input& Parameters::parse(int argc, char **argv) {

    if (argc < 1 || argv == nullptr)
        throw std::invalid_argument("invalid arg");

    opterr = 1;

    const char* short_options = "lhr";
    const struct option long_options[] = {
            { "help",     0, nullptr, 0 }
    };

    int option;
    while((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch(option) {
            case 0:
                input.result = ParseResult::HELP;
                return input;
            case 'l':
                input.flags |= longFlag;
                break;
            case 'h':
                input.flags |= humanFormatFlag;
                break;
            case 'r':
                input.flags |= descOrderFlag;
                break;
            default:
                input.result = ParseResult::ERROR_PARSE;
                return input;
        }
    }

    // Get directories and files
    for (int i=1; i < argc; ++i) {
        if (argv[i][0] != '-') {

            auto st = std::make_unique<struct stat>();
            if (lstat(argv[i], st.get()) != 0)
                throw std::runtime_error("lstat return error");

            if (S_ISDIR(st->st_mode))
                input.dirs.push_back(argv[i]);
            else {
                // Let's save the stat so as not to request twice
                input.files.emplace_back(argv[i], std::move(st));
            }
        }
    }

    // If directory/files are not specified, use the current one
    if (input.dirs.empty() && input.files.empty())
        input.dirs.push_back("./");

    return input;
}

Parameters::Parameters() = default;

Parameters &Parameters::getInstance() {
    static Parameters parameters;
    return parameters;
}

const Parameters::Input &Parameters::getInput() const {
    return input;
}
