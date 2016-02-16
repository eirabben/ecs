#pragma once

namespace ecs {

struct Id {
    Id() {}
    Id(unsigned int index, unsigned int counter) :
        index(index),
        counter(counter) {}

    unsigned int index {0};
    unsigned int counter {0};
};

struct Entity {
    const Id id;
    // ManagerType& manager;

    // Manager access functions
    // - Signature matching
    // - Component access
    // - Kill entity
};

}
