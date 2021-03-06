#pragma once

#include "Bitset.hpp"
#include "Util.hpp"
#include <tuple>
#include <vector>

namespace ecs {

template <typename... TArgs>
struct ComponentStorage {
    using DataIndex = unsigned int;

    // Stores one vector for each component type
    std::tuple<std::vector<TArgs>...> componentLists;

    template <typename T>
    void add(T component, DataIndex index) {
        get<T>()[index] = component;
    }

    template <typename T>
    std::vector<T>& get() {
        return std::get<std::vector<T>>(componentLists);
    }

    template <typename T>
    T& getComponent(DataIndex index) {
        return get<T>()[index];
    }

    template <typename T>
    void removeComponent(DataIndex index) {
        auto& v = get<T>();
        v.erase(v.begin() + index);
    }

    void removeEntity(DataIndex index) {
        util::forTuple(componentLists, [index](auto& v) {
            v.erase(v.begin() + index);
        });
    }

    void resize(std::size_t newCapacity) {
        util::forTuple(componentLists, [newCapacity](auto& v) {
            v.resize(newCapacity);
        });
    }
};

}
