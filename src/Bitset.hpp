#pragma once

#include <bitset>

namespace ecs {

constexpr int defaultBitsetSize {32};
using Bitset = std::bitset<defaultBitsetSize>;

}
