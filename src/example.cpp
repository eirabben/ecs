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
using Entity = ecs::Entity<Manager>;

struct PosProcessor {
    void update(Manager& mgr) {
        std::cout << "Updating PosProcessor\n";
        mgr.forEntitiesMatching<CPosition>(
        [](auto entity, auto& pos) {
            std::cout << "Index: " << entity.getIndex() << "\n";
        });
        std::cout << "\n";
    }
};

struct MulProcessor {
    void update(Manager& mgr) {
        std::cout << "Updating MulProcessor\n";
        mgr.forEntitiesMatching<CPosition, CVelocity>(
        [](auto entity, auto& pos, auto& vel) {
            std::cout << "Index: " << entity.getIndex() << "\n";
        });
        std::cout << "\n";
    }
};

int main() {
    // Create a manager object
    Manager mgr;

    // Create an entity, and add some components
    auto entity0 = mgr.createEntity()
        .add<CPosition>({1.0f, 0.1f})
        .add<CVelocity>({1.1});
    std::cout << "Entity0: " << entity0.getSignature() << "\n";

    auto entity1 = mgr.createEntity()
        .add<CVelocity>({2.2f})
        .add<CPosition>({2.0f, 0.2f});
    std::cout << "Entity1: " << entity1.getSignature() << "\n";

    auto entity2 = mgr.createEntity()
        .add<CVelocity>({3.3f});
    std::cout << "Entity2: " << entity2.getSignature() << "\n";

    auto entity3 = mgr.createEntity()
        .add<CPosition>({4.0f, 0.4f});
    std::cout << "Entity3: " << entity3.getSignature() << "\n";

    auto entity4 = mgr.createEntity();
    std::cout << "Entity4: " << entity4.getSignature() << "\n";

    std::cout << "\n";

    mgr.forEntities([](auto id) {
        std::cout << id.index << ", " << id.counter << "\n";
    });
    std::cout << "\n";

    entity0.kill();

    auto entity5 = mgr.createEntity()
        .add<CPosition>({});

    mgr.forEntities([](auto id) {
        std::cout << id.index << ", " << id.counter << "\n";
    });
    std::cout << "\n";


    // See if an entity has a component
    /* if (entity0.has<CVelocity>()) { */
    /*     std::cout << "Entity has component\n"; */
    /* } */

    // Access a component
    /* auto& pos = entity0.get<CPosition>(); */

    // Iterate over entities with lambdas
    PosProcessor pp;
    pp.update(mgr);
    MulProcessor mp;
    mp.update(mgr);

    // Remove a component
    /* entity0.remove<CVelocity>(); */

    // Remove an entity
    /* entity0.kill(); */

    return 0;
}
