#pragma once

#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "IdPool.hpp"
#include "Signature.hpp"
#include "TypeManager.hpp"

#include <initializer_list>
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <cassert>

namespace ecs {

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
        resizeIfNeeded(1);

        auto id = idPool.create();

        Entity entity {id};
        entities.push_back(entity);

        size++;

        return getEntity(id);
    }

    Entity& getEntity(unsigned int id) {
        assert(id < entities.size());

        auto it = std::find_if(entities.begin(), entities.end(),
            [id](const Entity& entity) {
                return id == entity.id;
            });

        return *it;
    }

    // @TODO: There is a difference between kill and destroy and remove
    void removeEntity(unsigned int id) {
        assert(id < entities.size());

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
        assert(id < entities.size());

        componentStorage.template add<T>(component, id);

        auto& entity = getEntity(id);
        auto componentType = typeManager.getTypeFor<T>();
        entity.signature[componentType.bitIndex] = true;
    }

    template <typename T>
    bool hasComponent(unsigned int id) {
        assert(id < entities.size());

        auto& entity = getEntity(id);
        auto componentType = typeManager.getTypeFor<T>();
        return entity.signature[componentType.bitIndex];
    }

    template <typename T>
    T& getComponent(unsigned int id) {
        assert(hasComponent<T>(id));
        return componentStorage.template getComponent<T>(id);
    }

    template <typename T>
    void removeComponent(unsigned int id) {
        assert(hasComponent<T>(id));

        componentStorage.template removeComponent<T>(id);

        auto& entity = getEntity(id);
        auto componentType = typeManager.getTypeFor<T>();
        entity.signature[componentType.bitIndex] = false;
    }

    template <typename... T>
    Signature getSignature() {
        // Using an initializer list to iterate over types. The bit of each type will be added to the bitset.
        Signature signature;
        auto list = {(signature = (signature | typeManager.getTypeFor<T>().bit))...};
        return signature;
    }

    template <typename... T>
    bool matchesSignature(Entity entity) {
        auto signature = getSignature<T...>();
        return (signature & entity.signature) == signature;
    }

    template <typename TF>
    void forEntities(TF&& function) {
        for (auto& entity : entities) {
            function(entity);
        }
    }

    template <typename... T, typename TF>
    void forEntitiesMatching(TF&& function) {
        forEntities([this, &function](auto& entity) {
            if (matchesSignature<T...>(entity)) {
                function(entity, getComponent<T...>(entity.id));
            }
        });
    }

    void resize(std::size_t newCapacity) {
        assert(newCapacity > capacity);

        // We now call push_back or emplace_back. This means the vector will
        // be resized each time an entity is added or removed.
        //entities.resize(newCapacity);

        componentStorage.resize(newCapacity);
        capacity = newCapacity;
    }

    void resizeIfNeeded(std::size_t numNewEntities) {
        if (size + numNewEntities > capacity) {
            resize(capacity + defaultCapacityIncrease);
        }
    }

private:
    std::size_t capacity {0};
    std::size_t size {0};

    // Entity vector
    // @TODO: This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states. Might use entity cache.
    std::vector<Entity> entities;

    // Component storage
    ComponentStorage<TArgs...> componentStorage;

    // Type manager
    TypeManager typeManager;

    // ID pool
    IdPool idPool;
};

}
