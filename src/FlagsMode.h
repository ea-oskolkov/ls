#ifndef LS_FLAGSMODE_H
#define LS_FLAGSMODE_H

#include <cstdint>

using FlagMode_t = uint8_t;

namespace FlagsMode {
    static constexpr FlagMode_t no = 0;
    static constexpr FlagMode_t longView = 1;
    static constexpr FlagMode_t descOrder = 2;
    static constexpr FlagMode_t humanSizeFormat = 4;
}

#endif //LS_FLAGSMODE_H
