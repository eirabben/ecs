#pragma once

#include "Entity.hpp"

namespace ecs {

template <typename TManager>
class Handle {
    friend TManager;

public:
    Handle(TManager& manager, const Entity entity) :
        entity {entity},
        manager {manager} {}

    template <typename... TComponents>
    auto matchesSignature() {
        return manager.template matchesSignature<TComponents...>(entity);
    }

    auto getSignature() {
        return manager.getSignature(entity);
    }

    template <typename TComponent>
    auto& addComponent(TComponent component) {
        manager.template addComponent<TComponent>(entity, component);
        return *this;
    }

    template <typename TComponent>
    auto hasComponent() {
        return manager.template hasComponent<TComponent>(entity);
    }

    template <typename TComponent>
    auto& getComponent() {
        return manager.template getComponent<TComponent>(entity);
    }

    template <typename TComponent>
    void removeComponent() {
        manager.template getComponent<TComponent>(entity);
    }

    void kill() {
        manager.killEntity(entity);
    }

private:
    TManager& manager;
    const Entity entity;
};

}
