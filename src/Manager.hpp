#pragma once

#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include "TypeManager.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>

constexpr int defaultCapacity{100};

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
        auto id = m_nextId++;

        Entity entity{id};
        entities[id] = entity;

        size++;

        return getEntity(id);
    }

    Entity& getEntity(unsigned int id) {
        return entities[id];
    }

    // @TODO: There is a difference between kill and destroy and remove
    void removeEntity(unsigned int id) {
        entities.erase(remove_if(std::begin(entities), std::end(entities), 
                    [id](const Entity& entity) {
                        return id == entity.id;
                    }),
                std::end(entities));
    }
    
    // COMPONENTS

    template <typename T>
    void addComponent(unsigned int id, T component) {
        m_componentStorage.template add<T>(component, id);

        auto& entity = getEntity(id);
        auto componentType = m_typeManager.getTypeFor<T>();
        entity.bitset[componentType.id] = true;
    }

    template <typename T>
    bool hasComponent(unsigned int id) {
        auto& entity = getEntity(id);
        auto componentType = m_typeManager.getTypeFor<T>();
        return entity.bitset[componentType.id];
    }

    template <typename T>
    T& getComponent(unsigned int id) {
        return m_componentStorage.template getComponent<T>(id);
    }

    template <typename T>
    void removeComponent(unsigned int id) {
        auto& entity = getEntity(id);
        auto componentType = m_typeManager.getTypeFor<T>();
        entity.bitset[componentType.id] = false;
        m_componentStorage.template removeComponent<T>(id);
    }


    void forEntities();
    void forEntitiesMatching();

    void resize(std::size_t newCapacity) {
        entities.resize(newCapacity);
        m_componentStorage.resize(newCapacity);

        capacity = newCapacity;
    }

    std::vector<Entity> entities;
private:
    std::size_t capacity{0};
    std::size_t size{0};
    unsigned int m_nextId{0};

    // Entity vector
    // This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states

    // Component storage
    ComponentStorage<TArgs...> m_componentStorage;

    // Type manager
    TypeManager m_typeManager;
};

