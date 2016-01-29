#pragma once

#include <bitset>
#include <string>

struct Entity {
    using EntityId = unsigned int;
    using DataIndex = unsigned int;

    Entity() {}
    Entity(EntityId id) : id(id) {}

    // The id.
    EntityId id;

    // The index in the data vectors
    DataIndex dataIndex;

    // What components and tags the entity has
    std::bitset<32> bitset{0};

    // Is the entity alive
    bool alive{true};
};
