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
        for (std::size_t i{1}; i < capacity; i++) {
            if (!inUse[i]) {
                inUse[i] = true;
                size++;
                return Id(i);
            }
        }
    }

    void remove(Id id) {
        inUse[id] = false;
        size--;
    }

    void resize(std::size_t newCapacity) {
        capacity = newCapacity; 
        inUse.resize(capacity);
    }

    void reset() {
        std::fill(inUse.begin(), inUse.end(), false);
        size = 0;
    }

private:
    std::size_t size {0};
    std::size_t capacity {0};
    std::vector<bool> inUse;
};

}
