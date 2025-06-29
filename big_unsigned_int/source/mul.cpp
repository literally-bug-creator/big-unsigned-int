#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
constexpr size_t SMALL_BYTE_LENGTH = 16384;
constexpr size_t MEDIUM_BYTE_LENGTH = 131072;
constexpr size_t CHUNK_BITS = sizeof(Chunk) * 8;
using MulChunk = __uint128_t;

BigUInt simpleMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    if (isZero(multiplicand) || isZero(multiplier)) {
        return BigUInt{};
    }

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
    return BigUInt{limbs};
}

BigUInt toomMul(const BigUInt& multiplicand, const BigUInt& multiplier) {}

BigUInt nntMul(const BigUInt& multiplicand, const BigUInt& multiplier) {}

}  // namespace
BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    size_t maxByteLength = std::max(getByteLength(multiplicand), getByteLength(multiplier));
    if (maxByteLength <= SMALL_BYTE_LENGTH) {
        return simpleMul(multiplicand, multiplier);
    }
    if (maxByteLength <= MEDIUM_BYTE_LENGTH) {
        return toomMul(multiplicand, multiplier);
    }
    return nntMul(multiplicand, multiplier);
}
}  // namespace big_uint
