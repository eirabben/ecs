#pragma once

#include <vector>
#include <tuple>
#include "Util.hpp"

/**
 * Stores one vector for each component type
 */
template <typename... TArgs>
struct ComponentStorage {
    // Stores one vector for each component type
    std::tuple<std::vector<TArgs>...> componentLists;

    template <typename T>
    void add(T component, unsigned int index) {
        get<T>()[index] = component;
    }

    template <typename T>
    std::vector<T>& get() {
        return std::get<std::vector<T>>(componentLists);
    }

    template <typename T>
    T& getComponent(unsigned int index) {
        return get<T>()[index];
    }

    template <typename T>
    void removeComponent(unsigned int index) {
        auto& v = get<T>();
        v.erase(std::begin(v) + index);
    }

    void resize(std::size_t newCapacity) {
        Util::forTuple(componentLists, [newCapacity](auto& vec) {
            vec.resize(newCapacity);
        });
    }
};
