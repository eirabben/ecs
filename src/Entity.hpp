#pragma once

#include <bitset>
#include <string>

struct Entity {

    Entity() {}
    Entity(unsigned int id) : id(id) {}

    // The index in the component storage vectors 
    // might not be needed
    //int dataIndex; 
    
    // The id.
    unsigned int id;

    // What components and tags the entity has
    std::bitset<32> bitset{0};

    // Is the entity alive
    bool alive{true};
};
