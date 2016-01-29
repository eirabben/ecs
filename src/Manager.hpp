#pragma once

#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "IdPool.hpp"
#include "TypeManager.hpp"
#include <vector>
#include <algorithm>
#include <utility>

constexpr int defaultCapacity {100};
constexpr int defaultCapacityIncrease {100};

template <typename... TArgs>
class Manager {

public:
    Manager() {
        resize(defaultCapacity);
    }

    // ENTITIES

    // @TODO: Decide if entities should be stored as a class or just IDs.
    // Also check if handles are a good thing to use for abstraction
    auto& createEntity() {
        resizeIfNeeded(size + 1);

        auto id = idPool.create();

        Entity entity {id};
        entities[id] = entity;

        size++;

        return getEntity(id);
    }

    Entity& getEntity(unsigned int id) {
        return entities[id];
    }

    // @TODO: There is a difference between kill and destroy and remove
    void removeEntity(unsigned int id) {
        entities.erase(remove_if(entities.begin(), entities.end(), 
                    [id](const Entity& entity) {
                        return id == entity.id;
                    }),
                entities.end());

        componentStorage.removeEntity(id);

        idPool.remove(id);
        size--;
    }
    
    // COMPONENTS

    template <typename T>
    void addComponent(unsigned int id, T component) {
        componentStorage.template add<T>(component, id);

        auto& entity = getEntity(id);
        auto componentType = typeManager.getTypeFor<T>();
        entity.bitset[componentType.bitIndex] = true;
    }

    template <typename T>
    bool hasComponent(unsigned int id) {
        auto& entity = getEntity(id);
        auto componentType = typeManager.getTypeFor<T>();
        return entity.bitset[componentType.bitIndex];
    }

    template <typename T>
    T& getComponent(unsigned int id) {
        return componentStorage.template getComponent<T>(id);
    }

    template <typename T>
    void removeComponent(unsigned int id) {
        auto& entity = getEntity(id);
        auto componentType = typeManager.getTypeFor<T>();
        entity.bitset[componentType.bitIndex] = false;
        componentStorage.template removeComponent<T>(id);
    }

    void forEntities() {
    
    }

    void forEntitiesMatching() {
    
    }

    void resize(std::size_t newCapacity) {
        entities.resize(newCapacity);
        componentStorage.resize(newCapacity);

        capacity = newCapacity;
    }

    void resizeIfNeeded(std::size_t newSize) {
        if (newSize > capacity) {
            resize(capacity + defaultCapacityIncrease);
        }
    }

    // @TODO: These are private. They are here for debugging.
    std::size_t capacity {0};
    std::size_t size {0};
private:

    // Entity vector
    // This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states
    std::vector<Entity> entities;

    // Component storage
    ComponentStorage<TArgs...> componentStorage;

    // Type manager
    TypeManager typeManager;

    // ID pool
    IdPool idPool;
};

