#include <cstddef>

#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
using MulChunk = __uint128_t;
constexpr size_t SMALL_BYTE_LENGTH = 16384;
constexpr size_t MEDIUM_BYTE_LENGTH = 131072;
constexpr size_t CHUNK_BITS = sizeof(Chunk) * 8;

std::vector<Chunk> removeTrailingZeros(const std::vector<Chunk>& limbs) {
    auto lastNonZero = static_cast<int64_t>(-1);
    for (int64_t index = static_cast<int64_t>(limbs.size()) - 1; index >= 0; index--) {
        if (limbs[static_cast<size_t>(index)] != 0) {
            lastNonZero = index;
            break;
        }
    }
    return {limbs.begin(), limbs.begin() + lastNonZero + 1};
}

BigUInt simpleMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    const std::vector<Chunk>& lhsLimbs = getLimbs(multiplicand);
    const std::vector<Chunk>& rhsLimbs = getLimbs(multiplier);
    size_t lhsSize = lhsLimbs.size();
    size_t rhsSize = rhsLimbs.size();
    std::vector<Chunk> limbs(lhsSize + rhsSize, 0);
    for (size_t i = 0; i < lhsSize; i++) {
        Chunk carry = 0;
        for (size_t j = 0; j < rhsSize; j++) {
            MulChunk product =
                (static_cast<MulChunk>(lhsLimbs[i]) * rhsLimbs[j]) + limbs[i + j] + carry;
            limbs[i + j] = static_cast<Chunk>(product);
            carry = static_cast<Chunk>(product >> CHUNK_BITS);
        }
        if (carry > 0) {
            limbs[i + rhsSize] += carry;
        }
    }
    return BigUInt{removeTrailingZeros(limbs)};
}

BigUInt karatsubaMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    return simpleMul(multiplicand, multiplier);
}

BigUInt nntMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    return simpleMul(multiplicand, multiplier);
}

}  // namespace

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    if (isZero(multiplicand) || isZero(multiplier)) {
        return makeZero();
    }
    size_t maxByteLength = std::max(getByteLength(multiplicand), getByteLength(multiplier));
    if (maxByteLength <= SMALL_BYTE_LENGTH) {
        return simpleMul(multiplicand, multiplier);
    }
    if (maxByteLength <= MEDIUM_BYTE_LENGTH) {
        return karatsubaMul(multiplicand, multiplier);
    }
    return nntMul(multiplicand, multiplier);
}
}  // namespace big_uint
