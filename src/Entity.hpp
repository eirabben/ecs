#pragma once

#include <bitset>

struct Entity {
    // The index in the component storage vectors 
    // might not be needed
    //int dataIndex; 

    // The id.
    int id;

    // What components and tags the entity has
    std::bitset<32> bitset; 

    // Is the entity alive
    bool alive;
};
