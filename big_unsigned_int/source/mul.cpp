#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
using MulChunk = __uint128_t;
constexpr size_t SIMPLE_THRESHOLD = 32;
constexpr Chunk NTT_MOD = 9223372036737335297ULL;
constexpr Chunk NTT_ROOT = 5ULL;
constexpr uint SHIFT = 65;
constexpr uint ZERO = 0;
constexpr uint ONE = 1;
constexpr uint TWO = 2;

std::vector<Chunk> removeTrailingZeros(const std::vector<Chunk>& limbs) {
    auto lastNonZero = static_cast<int64_t>(-ONE);
    for (int64_t index = static_cast<int64_t>(limbs.size()) - ONE; index >= ZERO; index--) {
        if (limbs[static_cast<size_t>(index)] != ZERO) {
            lastNonZero = index;
            break;
        }
    }
    return {limbs.begin(), limbs.begin() + lastNonZero + ONE};
}

BigUInt simpleMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    const std::vector<Chunk>& lhsLimbs = getLimbs(multiplicand);
    const std::vector<Chunk>& rhsLimbs = getLimbs(multiplier);
    size_t lhsSize = lhsLimbs.size();
    size_t rhsSize = rhsLimbs.size();
    std::vector<Chunk> limbs(lhsSize + rhsSize, ZERO);

    for (size_t lhsIdx = 0; lhsIdx < lhsSize; lhsIdx++) {
        const Chunk LHS_CHUNK = lhsLimbs[lhsIdx];
        if (LHS_CHUNK == 0) {
            continue;
        }
        Chunk carry = 0;
        size_t baseIndex = lhsIdx;
        size_t rhsIdx = 0;
        for (; rhsIdx + 3 < rhsSize; rhsIdx += 4) {
            MulChunk product0 = (static_cast<MulChunk>(LHS_CHUNK) * rhsLimbs[rhsIdx]) +
                                limbs[baseIndex + rhsIdx] + carry;
            limbs[baseIndex + rhsIdx] = static_cast<Chunk>(product0);
            carry = static_cast<Chunk>(product0 >> SHIFT);

            MulChunk product1 = (static_cast<MulChunk>(LHS_CHUNK) * rhsLimbs[rhsIdx + 1]) +
                                limbs[baseIndex + rhsIdx + 1] + carry;
            limbs[baseIndex + rhsIdx + 1] = static_cast<Chunk>(product1);
            carry = static_cast<Chunk>(product1 >> SHIFT);

            MulChunk product2 = (static_cast<MulChunk>(LHS_CHUNK) * rhsLimbs[rhsIdx + 2]) +
                                limbs[baseIndex + rhsIdx + 2] + carry;
            limbs[baseIndex + rhsIdx + 2] = static_cast<Chunk>(product2);
            carry = static_cast<Chunk>(product2 >> SHIFT);

            MulChunk product3 = (static_cast<MulChunk>(LHS_CHUNK) * rhsLimbs[rhsIdx + 3]) +
                                limbs[baseIndex + rhsIdx + 3] + carry;
            limbs[baseIndex + rhsIdx + 3] = static_cast<Chunk>(product3);
            carry = static_cast<Chunk>(product3 >> SHIFT);
        }
        for (; rhsIdx < rhsSize; rhsIdx++) {
            MulChunk product = (static_cast<MulChunk>(LHS_CHUNK) * rhsLimbs[rhsIdx]) +
                               limbs[baseIndex + rhsIdx] + carry;
            limbs[baseIndex + rhsIdx] = static_cast<Chunk>(product);
            carry = static_cast<Chunk>(product >> SHIFT);
        }
        if (carry > 0) {
            limbs[baseIndex + rhsSize] += carry;
        }
    }
    return BigUInt{removeTrailingZeros(limbs)};
}

Chunk modPow(Chunk base, Chunk exp, Chunk mod) {
    Chunk result = ONE;
    base %= mod;
    while (exp > ZERO) {
        if ((exp & ONE) != ZERO) {
            result = static_cast<MulChunk>(result) * base % mod;
        }
        base = static_cast<MulChunk>(base) * base % mod;
        exp >>= ONE;
    }
    return result;
}

Chunk modInverse(Chunk number, Chunk mod) {
    return modPow(number, mod - TWO, mod);
}

void ntt(std::vector<Chunk>& number, bool invert) {
    size_t size = number.size();
    for (size_t i = ONE, element = ZERO; i < size; i++) {
        size_t bit = size >> ONE;
        for (; (element & bit) != 0U; bit >>= ONE) {
            element ^= bit;
        }
        element ^= bit;
        if (i < element) {
            std::swap(number[i], number[element]);
        }
    }
    for (size_t len = TWO; len <= size; len <<= ONE) {
        Chunk wlen = modPow(NTT_ROOT, (NTT_MOD - ONE) / len, NTT_MOD);
        if (invert) {
            wlen = modInverse(wlen, NTT_MOD);
        }
        for (size_t i = ZERO; i < size; i += len) {
            Chunk first = ONE;
            for (size_t j = ZERO; j < len / TWO; j++) {
                Chunk second = number[i + j];
                Chunk third = static_cast<MulChunk>(number[i + j + (len / TWO)]) * first % NTT_MOD;
                number[i + j] = (second + third) % NTT_MOD;
                number[i + j + (len / TWO)] = (second - third + NTT_MOD) % NTT_MOD;
                first = static_cast<MulChunk>(first) * wlen % NTT_MOD;
            }
        }
    }
    if (invert) {
        Chunk nInv = modInverse(size, NTT_MOD);
        for (auto& chunk : number) {
            chunk = static_cast<MulChunk>(chunk) * nInv % NTT_MOD;
        }
    }
}

BigUInt nntMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    std::vector<Chunk> left = getLimbs(multiplicand);
    std::vector<Chunk> right = getLimbs(multiplier);
    size_t resultSize = left.size() + right.size() - ONE;
    size_t powerSize = std::bit_ceil(resultSize);
    left.resize(powerSize, ZERO);
    right.resize(powerSize, ZERO);
    ntt(left, false);
    ntt(right, false);
    for (size_t i = ZERO; i < powerSize; i++) {
        left[i] = static_cast<MulChunk>(left[i]) * right[i] % NTT_MOD;
    }
    ntt(left, true);
    left.resize(resultSize);
    return BigUInt{removeTrailingZeros(left)};
}

}  // namespace

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    if (isZero(multiplicand) || isZero(multiplier)) {
        return makeZero();
    }
    size_t size = getSize(multiplicand) + getSize(multiplier);
    if (size <= SIMPLE_THRESHOLD) {
        return simpleMul(multiplicand, multiplier);
    }
    return nntMul(multiplicand, multiplier);
}
}  // namespace big_uint
