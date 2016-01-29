#include <iostream>
#include <vector>

#include "Manager.hpp"

struct CPosition {

};

struct CVelocity {

};

void init();
void loop();

int main() {
    
    // Create the manager
    Manager<CPosition, CVelocity> mgr;

    // Create some components
    CPosition pos;
    CVelocity vel;

    // Create some entities and components
    auto& e1 = mgr.createEntity();
    mgr.addComponent<CPosition>(e1.id, {});

    auto& e2 = mgr.createEntity();
    mgr.addComponent<CPosition>(e2.id, {});
    mgr.addComponent<CVelocity>(e2.id, {});

    mgr.forEntities();

    if (mgr.hasComponent<CPosition>(e1.id)) {
        std::cout << "Yes\n";
    }

    mgr.removeComponent<CPosition>(e1.id);
    if (mgr.hasComponent<CPosition>(e1.id)) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
    }

    auto& e3 = mgr.getEntity(e2.id);

    auto& p = mgr.getComponent<CPosition>(e2.id);

    mgr.removeEntity(e1.id);

    mgr.forEntities();

    return 0;
}
