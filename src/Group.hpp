#pragma once

#include <vector>

namespace ecs {

template <typename... TComponents>
class Group {
    using EntityId = unsigned int;
    using EntityList = std::vector<EntityId>;

public:
    EntityList entities;

private:

}

}
