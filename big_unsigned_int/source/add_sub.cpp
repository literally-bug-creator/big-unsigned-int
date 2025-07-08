#include <algorithm>
#include <cstddef>

#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {

std::vector<Chunk> removeLeadingZeros(const std::vector<Chunk>& limbs) {
    if (limbs.empty()) {
        return {};
    }
    auto lastNonZero = static_cast<int64_t>(-ONE);
    for (int64_t i = static_cast<int64_t>(limbs.size()) - ONE; i >= ZERO; --i) {
        if (limbs[static_cast<size_t>(i)] != ZERO) {
            lastNonZero = i;
            break;
        }
    }
    if (lastNonZero == -ONE) {
        return {};
    }
    return {limbs.begin(), limbs.begin() + lastNonZero + 1};
}

BigUInt addInternal(const BigUInt& augend, const BigUInt& addend, size_t shift = 0) {
    const std::vector<Chunk>& leftLimbs = getLimbs(augend);
    const std::vector<Chunk>& rightLimbs = getLimbs(addend);
    size_t leftSize = leftLimbs.size();
    size_t rightSize = rightLimbs.size();
    size_t maxSize = std::max(leftSize + shift, rightSize);
    if (maxSize == 0) {
        return makeZero();
    }
    std::vector<Chunk> result;
    result.reserve(maxSize + ONE);
    Chunk carry = ZERO;
    for (size_t i = ZERO; i < maxSize; ++i) {
        Chunk leftChunk = 0;
        Chunk rightChunk = (i < rightSize) ? rightLimbs[i] : 0;
        if (i >= shift && (i - shift) < leftSize) {
            leftChunk = leftLimbs[i - shift];
        }
        Chunk sum = leftChunk + rightChunk + carry;
        if (sum > MAX_VALUE) {
            result.push_back(sum - MAX_VALUE - ONE);
            carry = 1;
        } else {
            result.push_back(sum);
            carry = 0;
        }
    }
    if (carry != 0) {
        result.push_back(carry);
    }
    return BigUInt{removeLeadingZeros(result)};
}

BigUInt subInternal(const BigUInt& minuend, const BigUInt& subtrahend, size_t shift = 0) {
    const std::vector<Chunk>& leftLimbs = getLimbs(minuend);
    const std::vector<Chunk>& rightLimbs = getLimbs(subtrahend);
    size_t leftSize = leftLimbs.size();
    size_t rightSize = rightLimbs.size();
    if (leftSize == 0 && rightSize == 0) {
        return makeZero();
    }
    size_t maxSize = std::max(leftSize + shift, rightSize);
    std::vector<Chunk> result;
    result.reserve(maxSize);
    Chunk borrow = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        Chunk leftChunk = 0;
        Chunk rightChunk = (i < rightSize) ? rightLimbs[i] : 0;
        if (i >= shift && (i - shift) < leftSize) {
            leftChunk = leftLimbs[i - shift];
        }
        if (borrow > leftChunk) {
            result.push_back(static_cast<Chunk>(MAX_VALUE + ONE + leftChunk - rightChunk - borrow));
            borrow = ONE;
        } else {
            leftChunk -= borrow;
            if (leftChunk < rightChunk) {
                result.push_back(static_cast<Chunk>(MAX_VALUE + ONE + leftChunk - rightChunk));
                borrow = ONE;
            } else {
                result.push_back(leftChunk - rightChunk);
                borrow = 0;
            }
        }
    }
    return BigUInt{removeLeadingZeros(result)};
}

}  // namespace

BigUInt add(const BigUInt& augend, const BigUInt& addend) {
    if (isZero(augend)) {
        return addend;
    }
    if (isZero(addend)) {
        return augend;
    }
    return addInternal(augend, addend);
}

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend) {
    if (isZero(subtrahend)) {
        return minuend;
    }
    if (isZero(minuend)) {
        return makeZero();
    }
    if (isEqual(minuend, subtrahend)) {
        return makeZero();
    }
    if (isLower(minuend, subtrahend)) {
        return makeZero();
    }
    return subInternal(minuend, subtrahend);
}

BigUInt add(const BigUInt& augend, const BigUInt& addend, size_t shift) {
    if (isZero(augend) && isZero(addend)) {
        return makeZero();
    }
    if (isZero(augend)) {
        return addend;
    }
    if (isZero(addend)) {
        if (shift == 0) {
            return augend;
        }
        const std::vector<Chunk>& augendLimbs = getLimbs(augend);
        std::vector<Chunk> shiftedLimbs(shift, 0);
        shiftedLimbs.insert(shiftedLimbs.end(), augendLimbs.begin(), augendLimbs.end());
        return BigUInt{removeLeadingZeros(shiftedLimbs)};
    }
    return addInternal(augend, addend, shift);
}

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend, size_t shift) {
    if (isZero(subtrahend)) {
        if (shift == 0) {
            return minuend;
        }
        const std::vector<Chunk>& minuendLimbs = getLimbs(minuend);
        std::vector<Chunk> shiftedLimbs(shift, 0);
        shiftedLimbs.insert(shiftedLimbs.end(), minuendLimbs.begin(), minuendLimbs.end());
        return BigUInt{removeLeadingZeros(shiftedLimbs)};
    }
    if (isZero(minuend)) {
        return makeZero();
    }
    BigUInt shiftedMinuend;
    if (shift == 0) {
        shiftedMinuend = minuend;
    } else {
        const std::vector<Chunk>& minuendLimbs = getLimbs(minuend);
        std::vector<Chunk> shiftedLimbs(shift, 0);
        shiftedLimbs.insert(shiftedLimbs.end(), minuendLimbs.begin(), minuendLimbs.end());
        shiftedMinuend = BigUInt{removeLeadingZeros(shiftedLimbs)};
    }
    if (isEqual(shiftedMinuend, subtrahend)) {
        return makeZero();
    }
    if (isLower(shiftedMinuend, subtrahend)) {
        return makeZero();
    }
    return subInternal(minuend, subtrahend, shift);
}

}  // namespace big_uint
