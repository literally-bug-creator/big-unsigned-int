#include "getters.hpp"

#include "big_uint.hpp"
#include "constants.hpp"

namespace big_uint {

bool isZero(const BigUInt& number) {
    if (getSize(number) == ZERO) {
        return true;
    }
    return (getSize(number) == ONE) && (getChunk(number, ZERO) == ZERO);
}

size_t getSize(const BigUInt& number) {
    return number.limbs.size();
}

size_t getByteLength(const BigUInt& number) {
    size_t limbAmount = getSize(number);
    if (limbAmount > SIZE_MAX / sizeof(Chunk)) {
        return SIZE_MAX;
    }
    return limbAmount * sizeof(Chunk);
}

Chunk getChunk(const BigUInt& number, size_t index) {
    if (index >= number.limbs.size()) {
        return ZERO_CHUNK;
    }
    return number.limbs[index];
}

const std::vector<Chunk>& getLimbs(const BigUInt& number) {
    return number.limbs;
}
}  // namespace big_uint
