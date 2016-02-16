#pragma once

#include <bitset>

constexpr int defaultBitsetSize {32};

namespace ecs {

using Bitset = std::bitset<defaultBitsetSize>;

}
