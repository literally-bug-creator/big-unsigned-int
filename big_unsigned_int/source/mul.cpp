#include <cstddef>
#include <cstdlib>

#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"
#include "iostream"

namespace big_uint {
namespace {
using MulChunk = __uint128_t;
constexpr size_t SIMPLE_THRESHOLD = 32;
constexpr size_t KARATSUBA_THRESHOLD = 2500;
constexpr Chunk NTT_MOD = 9223372036737335297ULL;
constexpr Chunk NTT_ROOT = 5ULL;
constexpr uint SHIFT = 64;

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
    for (size_t i = 0; i < lhsSize; i++) {
        const Chunk lhsChunk = lhsLimbs[i];
        if (lhsChunk == 0)
            continue;  // Пропускаем нулевые элементы

        Chunk carry = 0;
        size_t baseIndex = i;

        // Развернутый цикл для лучшей производительности
        size_t j = 0;
        for (; j + 3 < rhsSize; j += 4) {
            // Обрабатываем 4 элемента за раз
            MulChunk product0 =
                static_cast<MulChunk>(lhsChunk) * rhsLimbs[j] + limbs[baseIndex + j] + carry;
            limbs[baseIndex + j] = static_cast<Chunk>(product0 % (MAX_VALUE + 1));
            carry = static_cast<Chunk>(product0 / (MAX_VALUE + 1));

            MulChunk product1 = static_cast<MulChunk>(lhsChunk) * rhsLimbs[j + 1] +
                                limbs[baseIndex + j + 1] + carry;
            limbs[baseIndex + j + 1] = static_cast<Chunk>(product1 % (MAX_VALUE + 1));
            carry = static_cast<Chunk>(product1 / (MAX_VALUE + 1));

            MulChunk product2 = static_cast<MulChunk>(lhsChunk) * rhsLimbs[j + 2] +
                                limbs[baseIndex + j + 2] + carry;
            limbs[baseIndex + j + 2] = static_cast<Chunk>(product2 % (MAX_VALUE + 1));
            carry = static_cast<Chunk>(product2 / (MAX_VALUE + 1));

            MulChunk product3 = static_cast<MulChunk>(lhsChunk) * rhsLimbs[j + 3] +
                                limbs[baseIndex + j + 3] + carry;
            limbs[baseIndex + j + 3] = static_cast<Chunk>(product3 % (MAX_VALUE + 1));
            // carry = static_cast<Chunk>(product3 >> SHIFT) / (MAX_VALUE + 1);
            carry = static_cast<Chunk>(product3 / (MAX_VALUE + 1));
        }

        for (; j < rhsSize; j++) {
            MulChunk product =
                static_cast<MulChunk>(lhsChunk) * rhsLimbs[j] + limbs[baseIndex + j] + carry;
            limbs[baseIndex + j] = static_cast<Chunk>(product % (MAX_VALUE + 1));
            Chunk aboba = static_cast<Chunk>(product >> SHIFT) / (MAX_VALUE + 1);
            carry = static_cast<Chunk>(product / (MAX_VALUE + 1));
            // std::cout << carry - aboba << std::endl;
        }

        if (carry > 0) {
            limbs[baseIndex + rhsSize] += carry;
        }
    }

    return BigUInt{removeTrailingZeros(limbs)};
}

BigUInt karatsubaMul(const BigUInt& multiplicand, const BigUInt& multiplier) {  // NOLINT
    const std::vector<Chunk>& lhs = getLimbs(multiplicand);
    const std::vector<Chunk>& rhs = getLimbs(multiplier);
    size_t size = std::max(lhs.size(), rhs.size());
    if (size <= SIMPLE_THRESHOLD) {
        return simpleMul(multiplicand, multiplier);
    }
    size_t half = size / 2;
    std::vector<Chunk> aLow(lhs.begin(), lhs.begin() + std::min(half, lhs.size()));
    std::vector<Chunk> aHigh(lhs.begin() + std::min(half, lhs.size()), lhs.end());
    std::vector<Chunk> bLow(rhs.begin(), rhs.begin() + std::min(half, rhs.size()));
    std::vector<Chunk> bHigh(rhs.begin() + std::min(half, rhs.size()), rhs.end());
    BigUInt low = karatsubaMul(BigUInt{aLow}, BigUInt{bLow});
    BigUInt high = karatsubaMul(BigUInt{aHigh}, BigUInt{bHigh});
    BigUInt aSum = add(BigUInt{aLow}, BigUInt{aHigh});
    BigUInt bSum = add(BigUInt{bLow}, BigUInt{bHigh});
    BigUInt preResult = karatsubaMul(aSum, bSum);
    preResult = sub(preResult, high);
    preResult = sub(preResult, low);
    BigUInt result = low;
    result = add(low, shiftLeft(preResult, half * 64));
    result = add(result, shiftLeft(high, 2 * half * 64));
    return result;
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
    if (size <= KARATSUBA_THRESHOLD) {
        return karatsubaMul(multiplicand, multiplier);
    }
    return nntMul(multiplicand, multiplier);
}
}  // namespace big_uint
