#pragma once

#include "TypeManager.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>

#include "Entity.hpp"
#include "Component.hpp"

struct Signature {
    std::bitset<8> bits;
};

class Manager {
public:
    // Should return an EntityId or a handle to an entity 
    void createEntity() {
    
    }

    Entity& getEntity(unsigned int id) {
        return m_entities[id];
    }

    template <typename T, typename... TArgs>
    void addComponent(unsigned int id, TArgs&&... args) {
        auto& e = getEntity(id);
        
        e.bitset[m_typeManager.getTypeFor<T>().id] = true;

        // Should use unique_ptr, but it is not supported in clang here, apparently
        auto c = std::make_shared<T>(std::forward<TArgs>(args)...);

        // Add component to the correct component vector
    }

private:
    // Entity vector
    // This needs to be sorted by dead or alive, or we need multiple vectors
    // for different states
    std::vector<Entity> m_entities;

    // Component storage
    // We have one map for each Component type 
    // The size_t is the hash code for a component type, the int is the entity ID
    std::unordered_map< std::size_t, std::unordered_map<unsigned int, std::shared_ptr<Component> > > m_componentLists;

    TypeManager m_typeManager;
};
