#include <iostream>
#include <vector>

#include "Manager.hpp"

struct CPosition {
    float x {0};
    float y {0};
};

struct CVelocity {
    float vel {0};
};

using Manager = ecs::Manager<CPosition, CVelocity>;
using Entity = ecs::Entity;

struct PSystem {
    using Signature = ecs::Signature<CPosition>;

    void update(Manager& mgr) {
        mgr.forEntitiesMatching<Signature>(
        [](auto& entity, auto& pos) {
            std::cout << "Whaaat?\n";
            std::cout << entity.name << "\n";
            std::cout << pos.x << "\n";
        });
    }
};

int main() {
    // Create the manager
    Manager mgr;

    auto& e0 = mgr.createEntity();
    e0.name = "Entity 0";
    CPosition pos;
    pos.x = 4;
    mgr.addComponent<CPosition>(e0.id, pos);
    mgr.addComponent<CVelocity>(e0.id, {});

    auto& e1 = mgr.createEntity();
    e1.name = "Entity 1";
    mgr.addComponent<CPosition>(e1.id, {});

    auto& ePos = mgr.getComponent<CPosition>(e1.id);

    auto& e2 = mgr.createEntity();
    e2.name = "Entity 2";
    mgr.addComponent<CVelocity>(e2.id, {});

    PSystem sys;
    sys.update(mgr);

    return 0;
}
