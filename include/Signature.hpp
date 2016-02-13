#pragma once

#include "Bitset.hpp"
#include "TypeManager.hpp"
#include <initializer_list>

namespace ecs {

template <typename... Ts>
struct Signature {
    static Bitset getBitset() {
        Bitset bitset;
        auto list = {(bitset = (bitset | TypeManager::getTypeFor<Ts>().bit))...};
        return bitset;
    }
};

}
