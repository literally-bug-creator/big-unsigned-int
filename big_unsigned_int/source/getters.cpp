#include "getters.hpp"

#include "big_uint.hpp"

namespace big_uint {
constexpr Chunk ZERO = 0;

bool isZero(const BigUInt& number) {
    if (getLength(number) == 0) {
        return true;
    }
    return (getLength(number) == 1) && (getChunk(number, 0) == 0);
}

size_t getLength(const BigUInt& number) {
    return number.limbs.size();
}

size_t getByteLength(const BigUInt& number) {
    size_t limbAmount = getLength(number);
    if (limbAmount > SIZE_MAX / sizeof(Chunk)) {
        return SIZE_MAX;
    }
    return limbAmount * sizeof(Chunk);
}

Chunk getChunk(const BigUInt& number, size_t index) {
    if (index >= number.limbs.size()) {
        return ZERO;
    }
    return number.limbs[index];
}

const std::vector<Chunk>& getLimbs(const BigUInt& number) {
    return number.limbs;
}
}  // namespace big_uint
