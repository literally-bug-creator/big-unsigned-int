#include <cstdint>

#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
enum class Comparison : int8_t {
    GREATER = 1,
    EQUAL = 0,
    LOWER = -1,
};

Comparison compareByLength(const BigUInt& lhs, const BigUInt& rhs) {
    const size_t LHS_LENGTH = getSize(lhs);
    const size_t RHS_LENGTH = getSize(rhs);

    if (LHS_LENGTH > RHS_LENGTH) {
        return Comparison::GREATER;
    }
    if (LHS_LENGTH < RHS_LENGTH) {
        return Comparison::LOWER;
    }

    return Comparison::EQUAL;
}

Comparison compare(const BigUInt& lhs, const BigUInt& rhs) {
    Comparison byLength = compareByLength(lhs, rhs);
    if (byLength != Comparison::EQUAL) {
        return byLength;
    }
    for (size_t index = getSize(lhs); index-- > ZERO;) {
        Chunk lhsChunk = getChunk(lhs, index);
        Chunk rhsChunk = getChunk(rhs, index);

        if (lhsChunk > rhsChunk) {
            return Comparison::GREATER;
        }
        if (lhsChunk < rhsChunk) {
            return Comparison::LOWER;
        }
    }
    return Comparison::EQUAL;
}
}  // namespace

bool isEqual(const BigUInt& left, const BigUInt& right) {
    return compare(left, right) == Comparison::EQUAL;
}

bool isGreater(const BigUInt& left, const BigUInt& right) {
    return compare(left, right) == Comparison::GREATER;
}

bool isLower(const BigUInt& left, const BigUInt& right) {
    return compare(left, right) == Comparison::LOWER;
}

bool isGreaterOrEqual(const BigUInt& left, const BigUInt& right) {
    switch (compare(left, right)) {
        case Comparison::EQUAL:
        case Comparison::GREATER:
            return true;
        default:
            return false;
    }
}

bool isLowerOrEqual(const BigUInt& left, const BigUInt& right) {
    switch (compare(left, right)) {
        case Comparison::EQUAL:
        case Comparison::LOWER:
            return true;
        default:
            return false;
    }
}
}  // namespace big_uint
