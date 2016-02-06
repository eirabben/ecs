#pragma once

namespace ecs {

template <typename TManager>
struct Entity {
    using EntityId = unsigned int;
    EntityId id;

    Entity(EntityId id) : id(id) {}
};

}
