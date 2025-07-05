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

// NTT parameters
constexpr uint64_t NTT_MOD = 998244353;
constexpr uint64_t NTT_ROOT = 3;

// Fast modular exponentiation
uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (__uint128_t)result * base % mod;
        }
        base = (__uint128_t)base * base % mod;
        exp >>= 1;
    }
    return result;
}

void bitReverse(std::vector<uint64_t>& a) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; i++) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }
}

void ntt(std::vector<uint64_t>& a, bool inverse = false) {
    size_t n = a.size();
    if (n <= 1)
        return;

    bitReverse(a);

    for (size_t len = 2; len <= n; len <<= 1) {
        uint64_t root = modPow(NTT_ROOT, (NTT_MOD - 1) / len, NTT_MOD);
        if (inverse) {
            root = modPow(root, NTT_MOD - 2, NTT_MOD);
        }

        for (size_t i = 0; i < n; i += len) {
            uint64_t w = 1;
            for (size_t j = 0; j < len / 2; j++) {
                uint64_t u = a[i + j];
                uint64_t v = (__uint128_t)a[i + j + len / 2] * w % NTT_MOD;
                a[i + j] = (u + v) % NTT_MOD;
                a[i + j + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                w = (__uint128_t)w * root % NTT_MOD;
            }
        }
    }

    if (inverse) {
        uint64_t inv_n = modPow(n, NTT_MOD - 2, NTT_MOD);
        for (size_t i = 0; i < n; i++) {
            a[i] = (__uint128_t)a[i] * inv_n % NTT_MOD;
        }
    }
}

std::vector<uint64_t> chunksToNTT(const std::vector<Chunk>& chunks, size_t targetSize) {
    std::vector<uint64_t> result(targetSize, 0);

    constexpr size_t SPLIT_BITS = 30;
    constexpr uint64_t SPLIT_MASK = (1ULL << SPLIT_BITS) - 1;

    size_t pos = 0;
    for (size_t i = 0; i < chunks.size() && pos < targetSize; i++) {
        Chunk chunk = chunks[i];
        // Split into 30-bit parts
        result[pos++] = chunk & SPLIT_MASK;
        if (pos < targetSize) {
            result[pos++] = (chunk >> SPLIT_BITS) & SPLIT_MASK;
        }
        if (pos < targetSize && SPLIT_BITS * 2 < 64) {
            result[pos++] = (chunk >> (SPLIT_BITS * 2)) & SPLIT_MASK;
        }
    }

    return result;
}

std::vector<Chunk> nttToChunks(const std::vector<uint64_t>& nttResult) {
    constexpr size_t SPLIT_BITS = 30;
    constexpr uint64_t SPLIT_MASK = (1ULL << SPLIT_BITS) - 1;

    std::vector<Chunk> result;

    uint64_t carry = 0;
    for (size_t i = 0; i < nttResult.size(); i += 3) {
        uint64_t part0 = (i < nttResult.size()) ? nttResult[i] : 0;
        uint64_t part1 = (i + 1 < nttResult.size()) ? nttResult[i + 1] : 0;
        uint64_t part2 = (i + 2 < nttResult.size()) ? nttResult[i + 2] : 0;

        part0 += carry;
        carry = part0 >> SPLIT_BITS;
        part0 &= SPLIT_MASK;

        part1 += carry;
        carry = part1 >> SPLIT_BITS;
        part1 &= SPLIT_MASK;

        part2 += carry;
        carry = part2 >> SPLIT_BITS;
        part2 &= SPLIT_MASK;

        Chunk chunk = part0 | (part1 << SPLIT_BITS) | (part2 << (SPLIT_BITS * 2));
        result.push_back(chunk);

        carry = part2 >> (64 - SPLIT_BITS * 2);
    }

    while (carry > 0) {
        result.push_back(static_cast<Chunk>(carry));
        carry >>= 32;
    }

    return result;
}

size_t nextPowerOf2(size_t n) {
    if (n <= 1)
        return 1;
    size_t power = 1;
    while (power < n)
        power <<= 1;
    return power;
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

    size_t resultSize = lhsLimbs.size() + rhsLimbs.size();
    size_t nttSize = nextPowerOf2(resultSize * 3);

    std::vector<uint64_t> a = chunksToNTT(lhsLimbs, nttSize);
    std::vector<uint64_t> b = chunksToNTT(rhsLimbs, nttSize);

    ntt(a);
    ntt(b);

    for (size_t i = 0; i < nttSize; i++) {
        a[i] = (__uint128_t)a[i] * b[i] % NTT_MOD;
    }

    ntt(a, true);

    std::vector<Chunk> result = nttToChunks(a);

    return BigUInt{removeTrailingZeros(result)};
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
