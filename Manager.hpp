#pragma once

#include "Bitset.hpp"
#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "IdPool.hpp"
#include "TypeManager.hpp"

#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

constexpr std::size_t defaultCapacity {100};
constexpr std::size_t defaultCapacityIncrease {100};

namespace ecs {

template <typename... TArgs>
class Manager {
    using ThisType = Manager<TArgs...>;
    using EntityType = Entity<ThisType>;

public:
    Manager() {
        resize(defaultCapacity);
    }

    // ENTITIES

    auto createEntity() {
        resizeIfNeeded(1);

        Id id {idPool.create()};
        size++;

        entities.push_back(id);
        return getEntity(id);
    }

    auto getEntity(Id id) {
        return EntityType {*this, id};
    }

    auto getEntity(unsigned int index) {
        return getEntity(idPool.get(index));
    }

    void killEntity(Id id) {
        componentStorage.removeEntity(id.index);
        entitySignatures[id.index].reset();
        idPool.remove(id);

        entities.erase(std::remove(entities.begin(), entities.end(), id), entities.end());

        size--;
    }

    
    // COMPONENTS

    template <typename TComponent>
    void addComponent(Id id, TComponent component) {
        componentStorage.template add<TComponent>(component, id.index);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        entitySignatures[id.index][componentType.bitIndex] = true;
    }

    template <typename TComponent>
    bool hasComponent(Id id) {
        auto componentType = TypeManager::getTypeFor<TComponent>();
        return entitySignatures[id.index][componentType.bitIndex];
    }

    template <typename TComponent>
    TComponent& getComponent(Id id) {
        return componentStorage.template getComponent<TComponent>(id.index);
    }

    template <typename TComponent>
    void removeComponent(Id id) {
        componentStorage.template removeComponent<TComponent>(id.index);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        entitySignatures[id.index][componentType.bitIndex] = false;
    }

    // SIGNATURES

    auto getSignature(Id id) {
        return entitySignatures[id.index];
    }


    template <typename... TComponents>
    bool matchesSignature(Id id) {
        Bitset signature;
        auto list = {(signature = (signature | TypeManager::getTypeFor<TComponents>().bit))...};
        return (signature & getSignature(id)) == signature;
    }

    // Implementation borrowed from EntityX
    template <typename T> struct identity { using type = T; };

    void forEntities(typename identity<std::function<void(Id)>>::type function) {
        for (auto id : entities) {
            function(id);
        }
    }

    template <typename... TComponents>
    void forEntitiesMatching(typename identity<std::function<void(EntityType, TComponents&...)>>::type function) {
        forEntities([this, &function](auto id) {
            if (matchesSignature<TComponents...>(id)) {
                using Helper = ExpandCallHelper<TComponents...>;
                Helper::call(id, *this, function);
            }
        });
    }

    template <typename... Ts>
    struct ExpandCallHelper {
        static void call(Id id, ThisType& mgr, typename identity<std::function<void(EntityType entity, Ts&...)>>::type function) {
            function(mgr.getEntity(id), mgr.getComponent<Ts>(id)...);
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
    IdPool idPool;

    // Entity vector
    // @TODO: This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states. Might use entity cache.
    // Also, might implement handles.
    std::vector<Id> entities;
    std::vector<Bitset> entitySignatures;

    // Component storage
    ComponentStorage<TArgs...> componentStorage;


};

}
