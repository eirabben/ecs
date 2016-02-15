#pragma once

#include <algorithm>
#include <vector>

namespace ecs {

class IdPool {
using Id = unsigned int;

public:
    IdPool() {}
    IdPool(std::size_t initialCapacity) {
        resize(initialCapacity);
    }

    auto create() {
        Id id;
        for (std::size_t i{0}; i < capacity; i++) {
            if (!inUse[i]) {
                inUse[i] = true;
                id = i;
            }
        }
        return id;
    }

    void remove(Id id) {
        inUse[id] = false;
    }

    void resize(std::size_t newCapacity) {
        capacity = newCapacity; 
        inUse.resize(capacity);
    }

    void reset() {
        std::fill(inUse.begin(), inUse.end(), false);
    }

private:
    std::size_t capacity;
    std::vector<bool> inUse;
};

}
