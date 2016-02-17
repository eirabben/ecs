#pragma once

#include <vector>
#include <cassert>

#include "Entity.hpp"

namespace ecs {

constexpr int defaultPoolSize {100};

class IdPool {

public:
    IdPool() {
        resize(defaultPoolSize);
    }

    IdPool(std::size_t capacity) {
        resize(capacity);
    }

    auto create() {
        Id id;

        if (free.empty()) {
            id.index = nextIndex++;
            id.counter = 1;
            pool[id.index] = id;
        } else {
            unsigned int index = free.back();
            id = pool[index];
            free.pop_back();
        }

        return id;
    }

    void remove(Id id) {
        if (isValid(id)) {
            pool[id.index].counter++;
            free.push_back(id.index);
        }
    }

    auto get(unsigned int index) {
        assert(index < pool.size());
        assert(pool[index].counter > 0);

        return pool[index];
    }

    auto& getActive() {
        return pool;
    }

    bool isValid(Id id) {
        return (id.counter == pool[id.index].counter);
    }

    auto getCapacity() {
        return pool.size();
    }

    void resize(std::size_t capacity) {
        pool.resize(capacity);
    }

    void clear() {
        nextIndex = 0;
        pool.clear();
        free.clear();
        pool.resize(defaultPoolSize);
    }

private:
    unsigned int nextIndex {0};
    std::vector<Id> pool;
    std::vector<unsigned int> free;
};

}
