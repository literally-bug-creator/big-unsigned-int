#include <cstddef>

#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
using MulChunk = __uint128_t;
constexpr size_t LARGE_BYTE_LENGTH = 10000;
constexpr uint64_t NTT_MOD = 998244353ULL;
constexpr uint64_t NTT_ROOT = 3ULL;

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
    for (size_t i = ZERO; i < lhsSize; i++) {
        Chunk carry = ZERO;
        for (size_t j = ZERO; j < rhsSize; j++) {
            MulChunk product =
                (static_cast<MulChunk>(lhsLimbs[i]) * rhsLimbs[j]) + limbs[i + j] + carry;
            limbs[i + j] = static_cast<Chunk>(product % (MAX_VALUE + ONE));
            carry = static_cast<Chunk>(product / (MAX_VALUE + ONE));
        }
        if (carry > ZERO) {
            limbs[i + rhsSize] += carry;
        }
    }
    return BigUInt{removeTrailingZeros(limbs)};
}

uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = ONE;
    base %= mod;
    while (exp > ZERO) {
        if ((exp & (uint8_t)ONE) != 0U) {
            result = (__uint128_t)result * base % mod;
        }
        base = (__uint128_t)base * base % mod;
        exp >>= (uint8_t)ONE;
    }
    return result;
}

uint64_t modInverse(uint64_t number, uint64_t mod) {
    return modPow(number, mod - TWO, mod);
}

void ntt(std::vector<uint64_t>& number, bool invert) {
    size_t size = number.size();
    for (size_t i = ONE, element = ZERO; i < size; i++) {
        size_t bit = size >> (uint8_t)ONE;
        for (; (element & bit) != 0U; bit >>= (uint8_t)ONE) {
            element ^= bit;
        }
        element ^= bit;
        if (i < element) {
            std::swap(number[i], number[element]);
        }
    }
    for (size_t len = TWO; len <= size; len <<= (uint8_t)ONE) {
        uint64_t wlen = modPow(NTT_ROOT, (NTT_MOD - ONE) / len, NTT_MOD);
        if (invert) {
            wlen = modInverse(wlen, NTT_MOD);
        }
        for (size_t i = ZERO; i < size; i += len) {
            uint64_t first = ONE;
            for (size_t j = ZERO; j < len / TWO; j++) {
                uint64_t second = number[i + j];
                uint64_t third = (__uint128_t)number[i + j + (len / TWO)] * first % NTT_MOD;
                number[i + j] = (second + third) % NTT_MOD;
                number[i + j + (len / TWO)] = (second - third + NTT_MOD) % NTT_MOD;
                first = (__uint128_t)first * wlen % NTT_MOD;
            }
        }
    }
    if (invert) {
        uint64_t nInv = modInverse(size, NTT_MOD);
        for (auto& chunk : number) {
            chunk = (__uint128_t)chunk * nInv % NTT_MOD;
        }
    }
}

size_t nextPowerOf2(size_t n) {
    size_t power = ONE;
    while (power < n) {
        power <<= (uint8_t)ONE;
    }
    return power;
}

std::vector<uint64_t> chunksToNTT(const std::vector<Chunk>& chunks) {
    std::vector<uint64_t> result;
    result.reserve(chunks.size() * TWO);
    for (const Chunk& chunk : chunks) {
        result.push_back(chunk & 0xFFFFFFFFULL);
        result.push_back((chunk >> (uint8_t)32) & 0xFFFFFFFFULL);
    }
    return result;
}

std::vector<Chunk> nttToChunks(const std::vector<uint64_t>& nttResult) {
    std::vector<Chunk> result;
    result.reserve((nttResult.size() + ONE) / TWO);
    uint64_t carry = ZERO;
    for (size_t i = ZERO; i < nttResult.size(); i += TWO) {
        uint64_t low = (i < nttResult.size()) ? nttResult[i] : ZERO;
        uint64_t high = (i + ONE < nttResult.size()) ? nttResult[i + ONE] : ZERO;
        low += carry;
        carry = low >> (uint8_t)32;
        low &= 0xFFFFFFFFULL;
        high += carry;
        carry = high >> (uint8_t)32;
        high &= 0xFFFFFFFFULL;
        Chunk chunk = low | (high << (uint8_t)32);
        result.push_back(chunk);
    }
    while (carry > ZERO) {
        result.push_back(static_cast<Chunk>(carry));
        carry = ZERO;
    }
    return result;
}

BigUInt nntMul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    const std::vector<Chunk>& lhsLimbs = getLimbs(multiplicand);
    const std::vector<Chunk>& rhsLimbs = getLimbs(multiplier);
    if (lhsLimbs.empty() || rhsLimbs.empty()) {
        return makeZero();
    }
    if (lhsLimbs.size() + rhsLimbs.size() < 32) {
        return simpleMul(multiplicand, multiplier);
    }
    std::vector<uint64_t> left = chunksToNTT(lhsLimbs);
    std::vector<uint64_t> right = chunksToNTT(rhsLimbs);
    size_t resultSize = left.size() + right.size() - ONE;
    size_t powerSize = nextPowerOf2(resultSize);
    left.resize(powerSize, ZERO);
    right.resize(powerSize, ZERO);
    for (auto& chunk : left) {
        chunk %= NTT_MOD;
    }
    for (auto& chunk : right) {
        chunk %= NTT_MOD;
    }
    ntt(left, false);
    ntt(right, false);
    for (size_t i = ZERO; i < powerSize; i++) {
        left[i] = (__uint128_t)left[i] * right[i] % NTT_MOD;
    }
    ntt(left, true);
    left.resize(resultSize);
    std::vector<Chunk> resultChunks = nttToChunks(left);
    return BigUInt{removeTrailingZeros(resultChunks)};
}

}  // namespace

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier) {
    if (isZero(multiplicand) || isZero(multiplier)) {
        return makeZero();
    }
    size_t maxByteLength = std::max(getByteLength(multiplicand), getByteLength(multiplier));
    if (maxByteLength <= LARGE_BYTE_LENGTH) {
        return simpleMul(multiplicand, multiplier);
    }
    return nntMul(multiplicand, multiplier);
}
}  // namespace big_uint
