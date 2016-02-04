#pragma once

#include "Signature.hpp"
#include <string>

namespace ecs {

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
    Signature signature{0};

    // Is the entity alive
    bool alive{true};

    // Temporary name
    std::string name;
};

}
