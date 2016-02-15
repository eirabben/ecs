#pragma once

#include "Bitset.hpp"

namespace ecs {

struct Entity {
    using EntityId = unsigned int;

    Entity() {}
    Entity(EntityId id) : id(id) {}

    EntityId id;
    Bitset signature;
};

}
