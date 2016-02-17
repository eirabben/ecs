#pragma once

namespace ecs {

struct Id {
    Id() {}
    Id(unsigned int index, unsigned int counter) :
        index(index),
        counter(counter) {}

    bool operator==(const Id& other) const {
        return index == other.index && counter == other.counter;
    }

    // TODO: Could use a smaller size for these, and add bitwise operations. This would be good for performance and memory footprint.
    // Look here:
    // https://gamedev.stackexchange.com/questions/58376/allocating-entities-within-an-entity-system
    // http://gamesfromwithin.com/managing-data-relationships
    // https://gamedev.stackexchange.com/questions/33888/what-is-the-most-efficient-container-to-store-dynamic-game-objects-in<Paste>
    unsigned int index {0};
    unsigned int counter {0};
};

template <typename TManager>
class Entity {
public:
    Entity(TManager& manager, const Id id) :
        manager(manager),
        id(id) {}

    template <typename TComponent>
    auto& add(TComponent component) {
        manager.template addComponent<TComponent>(id, component);
        return *this;
    }

    template <typename TComponent>
    auto& get() {
        return manager.template getComponent<TComponent>(id);
    }

    template <typename TComponent>
    bool has() {
        return manager.template hasComponent<TComponent>(id);
    }

    template <typename TComponent>
    void remove() {
        manager.template removeComponent<TComponent>(id);
    }

    void kill() {
        manager.killEntity(id);
    }

    auto getSignature() {
        return manager.getSignature(id); 
    }

    auto getIndex() {
        return id.index; 
    }

private:
    const Id id;
    TManager& manager;

};

}
