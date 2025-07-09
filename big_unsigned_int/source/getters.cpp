#include "getters.hpp"

#include "big_uint.hpp"

namespace big_uint {
constexpr Chunk ZERO = 0;

bool isZero(const BigUInt& number) noexcept {
    if (getSize(number) == 0) {
        return true;
    }
    return (getSize(number) == 1) && (getLimb(number, 0) == 0);
}

size_t getSize(const BigUInt& number) noexcept {
    return number.limbs.size();
}

size_t getByteLength(const BigUInt& number) {
    size_t limbAmount = getSize(number);
    if (limbAmount > SIZE_MAX / sizeof(Chunk)) {
        return SIZE_MAX;
    }
    return limbAmount * sizeof(Chunk);
}

Chunk getLimb(const BigUInt& number, size_t index) {
    if (index >= number.limbs.size()) {
        return ZERO;
    }
    return number.limbs[index];
}

const std::vector<Chunk>& getLimbs(const BigUInt& number) {
    return number.limbs;
}
}  // namespace big_uint
