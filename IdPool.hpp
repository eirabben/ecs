#pragma once

#include "Entity.hpp"
#include <vector>

namespace ecs {

constexpr int defaultPoolSize {100};

class IdPool {
    using Index = unsigned int;

public:
    IdPool();
    IdPool(std::size_t capacity);

    // Ids
    Id create();
    void remove(Id id);
    Id get(Index index);
    bool isValid(Id id);

    // Pool
    std::size_t getCapacity();
    void resize(std::size_t capacity);
    void clear();

private:
    Index nextIndex {0};
    std::vector<Id> pool;
    std::vector<Index> free;
};

}
