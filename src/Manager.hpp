#pragma once

#include "Bitset.hpp"
#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "EntityHandle.hpp"
#include "IdPool.hpp"
#include "Settings.hpp"
#include "Signature.hpp"
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
    using EntityHandleType = EntityHandle<ThisType>;
    using EntityId = unsigned int;

public:
    Manager() {
        resize(defaultCapacity);
    }

    // ENTITIES

    auto createEntity() {
        resizeIfNeeded(1);

        auto id = idPool.create();
        Entity entity {id};
        entities[id] = entity;

        size++;

        return getEntity(id);
    }

    auto createHandle() {
        SlimHandle h;
        EntityHandleType eh {*this, h};
    }

    auto getEntity(EntityId id) {
        assert(id < size);

        return entities[id];
    }

    // @TODO: There is a difference between kill and destroy and remove
    void removeEntity(EntityId id) {
        assert(id < size);

        entities.erase(entities.begin() + id);

        componentStorage.removeEntity(id);
        idPool.remove(id);
        size--;
    }

    void removeEntity(Entity& entity) {
        removeEntity(entity.id);
    }
    
    // COMPONENTS

    template <typename TComponent>
    void addComponent(EntityId id, TComponent component) {
        assert(id < entities.size());

        componentStorage.template add<TComponent>(component, id);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        getEntity(id).signature[componentType.bitIndex] = true;
    }

    template <typename TComponent>
    bool hasComponent(EntityId id) {
        assert(id < entities.size());

        auto componentType = TypeManager::getTypeFor<TComponent>();
        return getEntity(id).signature[componentType.bitIndex];
    }

    template <typename TComponent>
    TComponent& getComponent(EntityId id) {
        return componentStorage.template getComponent<TComponent>(id);
    }

    template <typename TComponent>
    void removeComponent(EntityId id) {
        componentStorage.template removeComponent<TComponent>(id);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        getEntity(id).signature[componentType.bitIndex] = false;
    }

    // SIGNATURES

    template <typename... TComponents>
    bool matchesSignature(EntityId id) {
        Bitset signature;
        auto list = {(signature = (signature | TypeManager::getTypeFor<TComponents>().bit))...};
        auto entity = getEntity(id);
        return (signature & entity.signature) == signature;
    }

    // Implementation borrowed from EntityX
    template <typename T> struct identity { using type = T; };

    void forEntities(typename identity<std::function<void(EntityId)>>::type function) {
        for (auto entityId : entities) {
            /* function(entityId); */
        }
    }

    /* // Is this faster than passing a std::function? Can't be, right?*/
    /* template <typename TFunction> */
    /* void forEntities(TFunction&& function) { */
    /*     for (auto entity : entities) { */
    /*         function(entity); */
    /*     } */
    /* } */

    template <typename... TComponents>
    void forEntitiesMatching(typename identity<std::function<void(Entity, TComponents&...)>>::type function) {
        forEntities([this, &function](auto entityId) {
            if (matchesSignature<TComponents...>(entityId)) {
                using Helper = ExpandCallHelper<TComponents...>;
                Helper::call(entityId, *this, function);
            }
        });
    }

    template <typename... Ts>
    struct ExpandCallHelper {
        static void call(EntityId entityId, ThisType& mgr, typename identity<std::function<void(Entity entity, Ts&...)>>::type function) {
            function(mgr.getEntity(entityId), mgr.getComponent<Ts>(entityId)...);
        }
    };

    // MANAGER

    void resize(std::size_t newCapacity) {
        assert(newCapacity > capacity);

        capacity = newCapacity;
        idPool.resize(capacity);
        entities.resize(capacity);
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

    // Component storage
    ComponentStorage<TArgs...> componentStorage;


};

}
