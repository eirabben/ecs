#include "IdPool.hpp"
namespace ecs {

IdPool::IdPool() {
    resize(defaultPoolSize);
}

IdPool::IdPool(std::size_t capacity) {
    resize(capacity);
}

Id IdPool::create() {
    Id id;

    if (free.empty()) {
        id.index = nextIndex++;
        id.counter = 1;
        pool[id.index] = id;
    } else {
        Index index = free.back();
        id = pool[index];
        free.pop_back();
    }

    return id;
}

void IdPool::remove(Id id) {
    if (isValid(id)) {
        pool[id.index].counter++;
        free.push_back(id.index);
    }
}

Id IdPool::get(Index index) {
    assert(index < pool.size());
    assert(pool[index].counter > 0);

    return pool[index];
}

bool IdPool::isValid(Id id) {
    return (id.counter == pool[id.index].counter);
}

std::size_t IdPool::getCapacity() {
    return pool.size();
}

void IdPool::resize(std::size_t capacity) {
    pool.resize(capacity);
}

void IdPool::clear() {
    nextIndex = 0;
    pool.clear();
    free.clear();
    pool.resize(defaultPoolSize);
}

}
