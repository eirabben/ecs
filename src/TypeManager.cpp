#include "TypeManager.hpp"

namespace ecs {

std::unordered_map<std::size_t, Type> TypeManager::m_types;
BitIndex TypeManager::m_nextIndex {0};
Bitset TypeManager::m_nextBit = {1};

}
