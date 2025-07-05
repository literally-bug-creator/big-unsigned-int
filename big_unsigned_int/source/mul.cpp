#include <cstddef>

#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
using MulChunk = __uint128_t;
constexpr size_t LARGE_BYTE_LENGTH = 10000;
constexpr size_t CHUNK_BITS = sizeof(Chunk) * 8;
constexpr uint64_t NTT_MOD = 998244353ULL;
constexpr uint64_t NTT_ROOT = 3ULL;

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

uint64_t modInverse(uint64_t a, uint64_t mod) {
    return modPow(a, mod - 2, mod);
}

void ntt(std::vector<uint64_t>& a, bool invert) {
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
    for (size_t len = 2; len <= n; len <<= 1) {
        uint64_t wlen = modPow(NTT_ROOT, (NTT_MOD - 1) / len, NTT_MOD);
        if (invert) {
            wlen = modInverse(wlen, NTT_MOD);
        }
        for (size_t i = 0; i < n; i += len) {
            uint64_t w = 1;
            for (size_t j = 0; j < len / 2; j++) {
                uint64_t u = a[i + j];
                uint64_t v = (__uint128_t)a[i + j + len / 2] * w % NTT_MOD;
                a[i + j] = (u + v) % NTT_MOD;
                a[i + j + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                w = (__uint128_t)w * wlen % NTT_MOD;
            }
        }
    }
    if (invert) {
        uint64_t n_inv = modInverse(n, NTT_MOD);
        for (auto& x : a) {
            x = (__uint128_t)x * n_inv % NTT_MOD;
        }
    }
}

size_t nextPowerOf2(size_t n) {
    size_t power = 1;
    while (power < n) {
        power <<= 1;
    }
    return power;
}

std::vector<uint64_t> chunksToNTT(const std::vector<Chunk>& chunks) {
    std::vector<uint64_t> result;
    result.reserve(chunks.size() * 2);
    for (const Chunk& chunk : chunks) {
        result.push_back(chunk & 0xFFFFFFFFULL);
        result.push_back((chunk >> 32) & 0xFFFFFFFFULL);
    }
    return result;
}

std::vector<Chunk> nttToChunks(const std::vector<uint64_t>& nttResult) {
    std::vector<Chunk> result;
    result.reserve((nttResult.size() + 1) / 2);
    uint64_t carry = 0;
    for (size_t i = 0; i < nttResult.size(); i += 2) {
        uint64_t low = (i < nttResult.size()) ? nttResult[i] : 0;
        uint64_t high = (i + 1 < nttResult.size()) ? nttResult[i + 1] : 0;
        low += carry;
        carry = low >> 32;
        low &= 0xFFFFFFFFULL;
        high += carry;
        carry = high >> 32;
        high &= 0xFFFFFFFFULL;
        Chunk chunk = low | (high << 32);
        result.push_back(chunk);
    }
    while (carry > 0) {
        result.push_back(static_cast<Chunk>(carry));
        carry = 0;
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
    std::vector<uint64_t> a = chunksToNTT(lhsLimbs);
    std::vector<uint64_t> b = chunksToNTT(rhsLimbs);
    size_t result_size = a.size() + b.size() - 1;
    size_t n = nextPowerOf2(result_size);
    a.resize(n, 0);
    b.resize(n, 0);
    for (auto& x : a) {
        x %= NTT_MOD;
    }
    for (auto& x : b) {
        x %= NTT_MOD;
    }
    ntt(a, false);
    ntt(b, false);
    for (size_t i = 0; i < n; i++) {
        a[i] = (__uint128_t)a[i] * b[i] % NTT_MOD;
    }
    ntt(a, true);
    a.resize(result_size);
    std::vector<Chunk> resultChunks = nttToChunks(a);
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
