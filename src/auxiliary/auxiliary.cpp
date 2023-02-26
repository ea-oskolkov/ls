#include "auxiliary.h"

#include <sstream>
#include <iomanip>

namespace Auxiliary {

    static std::string doubleToString(long double value, int n) {
        std::stringstream ss;
        ss
            << std::fixed
            << std::setprecision(n)
            << value;
        return ss.str();
    }

    std::string getSizeDefaultFormat(uint64_t size) {
        return std::to_string(size);
    }

    std::string getSizeHumanFormat(uint64_t size) {
        static constexpr char unitsOfInformation[] =
                {
                        'K',
                        'M',
                        'G',
                        'T',
                        'P',
                        'E',
                        'Z',
                        'Y',
                        'R',
                        'Q'
                };

        static constexpr uint64_t divider = 1024ull;
        static constexpr int unitsArrMaxIdx = (int)(sizeof(unitsOfInformation) / sizeof(unitsOfInformation[0])) - 1;

        if (size < divider)
            return getSizeDefaultFormat(size);

        std::string ret;

        int arrIdx = -1;
        long double value = size;

        while (value >= divider && arrIdx < unitsArrMaxIdx) {
            value /= (long double) divider;
            ++arrIdx;
        }

        ret = doubleToString(value, 1);
        if (arrIdx != -1)
            ret.append(1, unitsOfInformation[arrIdx]);

        return ret;
    }
}
