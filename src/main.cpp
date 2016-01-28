#include <iostream>

#include "Manager.hpp"
#include "IdPool.hpp"

struct CPosition {

};

struct CVelocity {

};

int main() {

    IdPool pool;

    for (int i{0}; i < 100; i++) {
        auto id = pool.create();
        std::cout << id << "\n";
    }
    
    //// Create the manager
    //Manager<CPosition, CVelocity> mgr;

    //std::cout << "Initial mgr size: " << mgr.entities.size() << "\n";

    //// Create an entity
    //Entity& entity = mgr.createEntity();
    //std::cout << entity.id << "\n";
    //std::cout << entity.bitset << "\n";

    //std::cout << "After add mgr size: " << mgr.entities.size() << "\n";

    //// Add a component
    //CPosition pos;
    //CVelocity vel;
    //mgr.addComponent<CPosition>(entity.id, pos);
    //mgr.addComponent<CVelocity>(entity.id, vel);
    //std::cout << entity.bitset << "\n";

    //// Check for component
    //if (mgr.hasComponent<CPosition>(entity.id)) {
        //std::cout << "Has position\n";
    //}

    //// Remove component
    //mgr.removeComponent<CVelocity>(entity.id);
    //std::cout << entity.bitset << "\n";

    //mgr.removeEntity(entity.id);

    return 0;
}
