#pragma once

#include <unordered_map>
#include <bitset>
#include <typeindex>
#include <utility>

// @TODO: Use bitset size from settings

using TypeId = unsigned int;
using Bitset = std::bitset<8>;

struct Type {
    Type(TypeId id, Bitset bit) : id(id), bit(bit) {}

    TypeId id;
    Bitset bit;
};

class TypeManager {
public:
    template <typename T>
    Type getTypeFor() {
        auto typeHash = std::type_index(typeid(T)).hash_code();

        auto it = m_types.find(typeHash);
        if (it == m_types.end()) {
            Type type(m_nextId, m_nextBit);

            m_nextId++;
            m_nextBit = m_nextBit << 1;

            m_types.insert(std::make_pair(typeHash, type));

            return type;
        } else {
            return it->second;
        }
    }

private:
    std::unordered_map<std::size_t, Type> m_types;

    TypeId m_nextId = 0;
    Bitset m_nextBit = 1;
};
