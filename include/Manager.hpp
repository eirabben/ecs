#pragma once

#include "Bitset.hpp"
#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "IdPool.hpp"
#include "Settings.hpp"
#include "Signature.hpp"
#include "TypeManager.hpp"

#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

namespace ecs {

template <typename... TArgs>
class Manager {
using ThisType = Manager<TArgs...>;
using EntityId = unsigned int;
using Entity = Entity<ThisType>;

public:
    Manager() {
        resize(defaultCapacity);
    }

    // ENTITIES

    // @TODO: Decide if entities should be stored as a class or just IDs.
    // Also check if handles are a good thing to use for abstraction
    auto createEntity() {
        resizeIfNeeded(1);

        auto id = idPool.create();
        entities.push_back(id);

        size++;

        return getEntity(id);
    }

    auto getEntity(EntityId id) {
        assert(id < entities.size());

        Entity entity {id, this};
        return entity;
    }

    // @TODO: There is a difference between kill and destroy and remove
    void removeEntity(EntityId id) {
        assert(id < entities.size());

        entities.erase(std::remove(entities.begin(), entities.end(), id), entities.end());

        componentStorage.removeEntity(id);
        entityBitsets[id].reset();
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
        entityBitsets[id][componentType.bitIndex] = true;
    }

    template <typename TComponent>
    bool hasComponent(EntityId id) {
        assert(id < entities.size());

        auto componentType = TypeManager::getTypeFor<TComponent>();
        return entityBitsets[id][componentType.bitIndex];
    }

    template <typename TComponent>
    TComponent& getComponent(EntityId id) {
        assert(hasComponent<TComponent>(id));
        return componentStorage.template getComponent<TComponent>(id);
    }

    template <typename TComponent>
    void removeComponent(EntityId id) {
        assert(hasComponent<TComponent>(id));

        componentStorage.template removeComponent<TComponent>(id);

        auto componentType = TypeManager::getTypeFor<TComponent>();
        entityBitsets[id][componentType.bitIndex] = false;
    }

    // SIGNATURES

    template <typename TSignature>
    bool matchesSignature(EntityId id) {
        auto bitset = TSignature::getBitset();
        return (bitset & entityBitsets[id]) == bitset;
    }

    template <typename TFunction>
    void forEntities(TFunction&& function) {
        for (auto entity : entities) {
            function(entity);
        }
    }

    template <typename TSignature, typename TFunction>
    void forEntitiesMatching(TFunction&& function) {
        forEntities([this, &function](auto& entity) {
            if (matchesSignature<TSignature>(entity)) {
                expandSignatureCall<TSignature>(entity, function);
            }
        });
    }

    // MANAGER

    void resize(std::size_t newCapacity) {
        assert(newCapacity > capacity);

        // We now call push_back or emplace_back. This means the vector will
        // be resized each time an entity is added or removed.
        //entities.resize(newCapacity);

        componentStorage.resize(newCapacity);
        capacity = newCapacity;

        entityBitsets.resize(newCapacity);
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
    std::vector<EntityId> entities;

    // Stores the bitset for each entity
    std::vector<Bitset> entityBitsets;

    // Component storage
    ComponentStorage<TArgs...> componentStorage;

    // ID pool
    IdPool idPool;

    template <typename... Ts>
    struct ExpandCallHelper;

    template <typename T, typename TF>
    void expandSignatureCall(EntityId id, TF&& function) {
        using Helper = util::Rename<ExpandCallHelper, T>;
        Helper::call(getEntity(id), *this, function);
    }

    template <typename... Ts>
    struct ExpandCallHelper {
        template <typename TF>
        static void call(Entity entity, ThisType& mgr, TF&& function) {
            function(entity, mgr.getComponent<Ts>(entity.id)...);
        }
    };
};

}
