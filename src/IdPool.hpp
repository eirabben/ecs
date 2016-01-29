#pragma once

#include <vector>

/**
 * Keeps track of used and unused IDs, helps avoid making new IDs when this
 * is not needed.
 */
class IdPool {
    using EntityId = unsigned int;

public:
    EntityId create() {
        EntityId id;

        if (free.empty()) {
            id = nextId++;
            pool.push_back(id);
        } else {
            id = free.back();
            pool.push_back(id);
            free.pop_back();
        }

        return id;
    }

    void remove(EntityId id) {
        free.push_back(id);
        pool.erase(std::remove(pool.begin(), pool.end(), id), pool.end());
    }

    void resize(std::size_t newCapacity) {
        // @TODO: push_back and pop_back will change vector size
    }

    void reset() {
        nextId = 0;
        pool.clear();
        free.clear();
    }

private:
    EntityId nextId {0};
    // std::size_t capacity {0};

    std::vector<EntityId> pool;
    std::vector<EntityId> free;

};
