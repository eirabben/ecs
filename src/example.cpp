#include <iostream>
#include <vector>

#include "Manager.hpp"

struct CPosition {
    float x {0};
    float y {0};

    CPosition() {}
    CPosition(float x, float y) : x(x), y(y) {}
};

struct CVelocity {
    float vel {0};

    CVelocity() {}
    CVelocity(float vel) : vel(vel) {}
};

using Manager = ecs::Manager<CPosition, CVelocity>;

struct Processor {
    void update(Manager& mgr) {
        mgr.forEntitiesMatching<CPosition>(
        [](auto entity, auto& pos) {
            std::cout << pos.x << ", " << pos.y << "\n";
        });
    }
};

using IdPool = ecs::IdPool;

int main() {
    // Create a manager object
    Manager manager;

    manager.createHandle();

    // Create an entity, and add some components
    /* auto& entity = manager.createEntity(); */
    /* manager.addComponent<CPosition>(entity.id, {1.0f, 4.0f}); */
    /* manager.addComponent<CVelocity>(entity.id, {}); */

    /* auto& entity2 = manager.createEntity(); */
    /* manager.addComponent<CPosition>(entity.id, {}); */

/*     // See if an entity has a component */
/*     if (entity.hasComponent<CVelocity>()) { */
/*         // Do stuff */
/*     } */

/*     // Access a component */
/*     auto& pos = entity.getComponent<CPosition>(); */

/*     // Remove a component */
/*     entity.removeComponent<CVelocity>(); */

    // Iterate over entities with lambdas
    /* Processor processor; */
    /* processor.update(manager); */

/*     // Remove an entity */
/*     manager.removeEntity(entity); */

    return 0;
}
