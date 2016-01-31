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

    // Create some entities and components
    auto& e1 = mgr.createEntity();
    mgr.addComponent<CPosition>(e1.id, {});

    auto& e2 = mgr.createEntity();
    mgr.addComponent<CPosition>(e2.id, {});
    mgr.addComponent<CVelocity>(e2.id, {});

    //mgr.forEntities([](auto& entity) {
        //std::cout << "ID: " << entity.id << "\n";
        //std::cout << "Bitset: " << entity.bitset << "\n";
    //});

    std::bitset<32> bitset {1};
    std::cout << "InnnBitset: " << bitset << "\n";
    mgr.forEntitiesMatching(bitset, [](auto& entity) {
        std::cout << "ID: " << entity.id << "\n";
        std::cout << "Bitset: " << entity.bitset << "\n";
    });




    return 0;
}
