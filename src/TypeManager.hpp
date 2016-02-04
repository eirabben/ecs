#pragma once

#include "Signature.hpp"
#include <unordered_map>
#include <typeindex>
#include <utility>

namespace ecs {

using BitIndex = unsigned int;

struct Type {
    Type(BitIndex bitIndex, Signature bit) : bitIndex(bitIndex), bit(bit) {}

    BitIndex bitIndex;
    Signature bit;
};

class TypeManager {
public:
    template <typename T>
    Type getTypeFor() {
        auto typeHash = std::type_index(typeid(T)).hash_code();

        auto it = m_types.find(typeHash);
        if (it == m_types.end()) {
            Type type(m_nextIndex, m_nextBit);

            m_nextIndex++;
            m_nextBit = m_nextBit << 1;

            m_types.insert(std::make_pair(typeHash, type));

            return type;
        } else {
            return it->second;
        }
    }

private:
    std::unordered_map<std::size_t, Type> m_types;

    BitIndex m_nextIndex = 0;
    Signature m_nextBit = 1;
};

}
