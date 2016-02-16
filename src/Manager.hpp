#pragma once

#include "Bitset.hpp"
#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "Handle.hpp"
#include "IdPool.hpp"
#include "TypeManager.hpp"

#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

#include <iostream>

constexpr std::size_t defaultCapacity {100};
constexpr std::size_t defaultCapacityIncrease {100};

namespace ecs {

template <typename... TArgs>
class Manager {
    using ThisType = Manager<TArgs...>;
    using HandleType = Handle<ThisType>;

public:
    Manager() {
        resize(defaultCapacity);
    }

    // HANDLES

    auto createHandle() {
        HandleType handle{*this, createEntity()};
        return handle;
    }

    auto getHandle(unsigned int id) {
        return getHandle(getEntity(id));
    }

    auto getHandle(Entity entity) {
        HandleType handle{*this, entity};
        return handle;
    }

    // ENTITIES

    auto createEntity() {
        resizeIfNeeded(1);

        Entity entity {idPool.create()};
        entities[entity.id] = entity;

        size++;
        return getEntity(entity.id);
    }

    auto getEntity(unsigned int id) {
        return entities[id];
    }

    void killEntity(Entity entity) {
        componentStorage.removeEntity(entity.id);
        entitySignatures[entity.id].reset();
        idPool.remove(entity.id);

        entities.erase(entities.begin() + entity.id);
        size--;
    }

    void killEntity(HandleType& handle) {
        killEntity(handle.entity);
    }

    Bitset getSignature(Entity entity) {
        return entitySignatures[entity.id];
    }

    
    // COMPONENTS

    template <typename TComponent>
    void addComponent(Entity entity, TComponent component) {
        componentStorage.template add<TComponent>(component, entity.id);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        entitySignatures[entity.id][componentType.bitIndex] = true;
    }

    template <typename TComponent>
    bool hasComponent(Entity entity) {
        auto componentType = TypeManager::getTypeFor<TComponent>();
        return entitySignatures[entity.id][componentType.bitIndex];
    }

    template <typename TComponent>
    TComponent& getComponent(Entity entity) {
        return componentStorage.template getComponent<TComponent>(entity.id);
    }

    template <typename TComponent>
    void removeComponent(Entity entity) {
        componentStorage.template removeComponent<TComponent>(entity.id);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        entitySignatures[entity.id][componentType.bitIndex] = false;
    }

    // SIGNATURES

    template <typename... TComponents>
    bool matchesSignature(Entity entity) {
        std::cout << "Checking match\n";
        Bitset signature;
        auto list = {(signature = (signature | TypeManager::getTypeFor<TComponents>().bit))...};
        return (signature & entitySignatures[entity.id]) == signature;
    }

    // Implementation borrowed from EntityX
    template <typename T> struct identity { using type = T; };

    void forEntities(typename identity<std::function<void(Entity)>>::type function) {
        std::cout << "forEntities\n";
        for (auto entity : entities) {
            function(entity);
        }
    }

    template <typename... TComponents>
    void forEntitiesMatching(typename identity<std::function<void(HandleType, TComponents&...)>>::type function) {
        std::cout << "forEntitiesMatching\n";
        forEntities([this, &function](auto entity) {
            if (matchesSignature<TComponents...>(entity)) {
                std::cout << "Found match\n";
                using Helper = ExpandCallHelper<TComponents...>;
                Helper::call(entity, *this, function);
            }
        });
    }

    template <typename... Ts>
    struct ExpandCallHelper {
        static void call(Entity entity, ThisType& mgr, typename identity<std::function<void(HandleType entity, Ts&...)>>::type function) {
            function(mgr.getHandle(entity), mgr.getComponent<Ts>(entity)...);
        }
    };

    // MANAGER

    void resize(std::size_t newCapacity) {
        assert(newCapacity > capacity);

        capacity = newCapacity;
        idPool.resize(capacity);
        entities.reserve(capacity);
        entitySignatures.resize(capacity);
        componentStorage.resize(capacity);
    }

    void resizeIfNeeded(std::size_t numNewEntities) {
        if (size + numNewEntities > capacity) {
            resize(capacity + defaultCapacityIncrease);
        }
    }

private:
    std::size_t capacity {0};
    std::size_t size {0};
    IdPool idPool {0};

    // Entity vector
    // @TODO: This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states. Might use entity cache.
    // Also, might implement handles.
    std::vector<Entity> entities;
    std::vector<Bitset> entitySignatures;

    // Component storage
    ComponentStorage<TArgs...> componentStorage;


};

}
