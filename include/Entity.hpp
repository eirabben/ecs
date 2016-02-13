#pragma once

namespace ecs {

template <typename TManager>
struct Entity {
    using EntityId = unsigned int;
    using Manager = TManager;

    Entity(EntityId id, Manager* manager) : id(id), manager(manager) {}

    template <typename TComponent>
    Entity& addComponent(TComponent component) {
        manager->template addComponent<TComponent>(id, component);
        return *this;
    }

    template <typename TComponent>
    TComponent& getComponent() {
        return manager->template getComponent<TComponent>(id);
    }

    template <typename TComponent>
    bool hasComponent() {
        return manager->template hasComponent<TComponent>(id);
    }

    template <typename TComponent>
    void removeComponent() {
        manager->template removeComponent<TComponent>(id);
    }

    EntityId id;
    Manager* manager;
};

}
