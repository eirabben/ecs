#pragma once

#include <vector>

class IdPool {
    using EntityId = unsigned int;

public:
    EntityId create() {
        EntityId id;

        if (free.empty()) {
            id = nextId++;
            ids.push_back(id);
        } else {
            id = free.back();
            ids.push_back(id);
            free.pop_back();
        }

        return id;
    }

    void remove(EntityId id) {
        free.push_back(id);
        ids.erase(std::remove(std::begin(ids), std::end(ids), id), std::end(ids));
    }

    void reset() {
        nextId = 0;
        ids.clear();
        free.clear();
    }

private:
    EntityId nextId{0};

    std::vector<EntityId> ids;
    std::vector<EntityId> free;

};
