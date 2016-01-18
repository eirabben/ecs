#pragma once

#include <bitset>

struct Entity {
    // The index in the component storage vectors 
    int dataIndex; 

    // What components and tags the entity has
    std::bitset<32> bitset; 

    // Is the entity alive
    bool alive; 
};
