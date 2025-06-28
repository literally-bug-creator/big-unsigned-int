#include "tools.hpp"

#include <utility>

using namespace big_uint;

BigUInt createTestBigUInt(std::vector<Chunk> limbs) {
    return BigUInt(std::move(limbs));
}
