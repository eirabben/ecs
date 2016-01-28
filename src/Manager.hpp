#pragma once

#include "TypeManager.hpp"
#include <vector>
#include <tuple>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <utility>

#include "Entity.hpp"
#include "Component.hpp"

template <typename... TArgs>
struct ComponentStorage {
    // Stores one vector for each component type
    std::tuple<std::vector<TArgs>...> componentLists;

    template <typename T>
    void add(T component) {
        get<T>().push_back(component);
    }

    template <typename T>
    std::vector<T>& get() {
        return std::get<std::vector<T>>(componentLists);
    }

};

struct Signature {
    std::bitset<8> bits;
};

template <typename... TArgs>
class Manager {

public:
    // ENTITIES

    // @TODO: Decide if entities should be stored as a class or just IDs.
    // Also check if handles are a good thing to use for abstraction
    Entity& createEntity() {
        auto id = m_nextId++;

        Entity entity {id};
        entity.bitset.reset();
        entity.alive = true;

        m_entities.push_back(entity);

        return getEntity(id);
    }

    Entity& getEntity(unsigned int id) {
        auto it = std::find_if(m_entities.begin(), m_entities.end(), [id](const Entity& entity) {
            return id == entity.id;
        });

        // @TODO: This will cause errors if entity does not exist
        return *it;
    }

    // @TODO: There is a difference between kill and destroy and remove
    void removeEntity(unsigned int id) {

    }
    
    // COMPONENTS

    template <typename T, typename... Args>
    void addComponent(unsigned int id, Args&&... args) {
        auto componentType = m_typeManager.getTypeFor<T>();

        auto& entity = getEntity(id);
        entity.bitset[componentType.id] = true;

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    bool hasComponent(unsigned int id) {
    
    }

    template <typename T>
    T& getComponent(unsigned int id) {
        // Implementation detail. Abstract this
        auto componentType = m_typeManager.getTypeFor<T>();

    }

    template <typename T>
    void removeComponent(unsigned int id) {
    
    }

    void forEntities();
    void forEntitiesMatching();

private:
    // Entity vector
    // This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states
    std::vector<Entity> m_entities;
    unsigned int m_nextId = 1;

    // Component storage NEW
    ComponentStorage<TArgs...> m_componentStorage;

    TypeManager m_typeManager;
};

