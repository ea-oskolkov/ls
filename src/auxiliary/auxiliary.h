#ifndef LS_AUXILIARY_H
#define LS_AUXILIARY_H

#include <string>
#include <cstdint>

namespace Auxiliary {
    std::string getSizeDefaultFormat(uint64_t size);
    std::string getSizeHumanFormat(uint64_t size);
}

#endif //LS_AUXILIARY_H
