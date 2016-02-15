#pragma once

namespace ecs {


class SlimHandle {
    using HandleDataIndex = std::size_t;
    using Counter = int;
public:
    HandleDataIndex handleDataIndex;
    Counter counter;
};

template <typename TManager>
class EntityHandle {
    friend TManager;
public:
    EntityHandle(TManager& manager, const SlimHandle& slimHandle) :
        slimHandle {slimHandle},
        manager {manager} {}

private:
    TManager& manager;
    const SlimHandle slimHandle;
};

}
