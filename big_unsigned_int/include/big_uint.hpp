#pragma once

#include <cstdint>
#include <vector>

namespace big_uint {
using Chunk = uint64_t;

struct BigUInt {
    std::vector<Chunk> limbs;
};

BigUInt add(const BigUInt& augend, const BigUInt& addend);

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend);

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier);

BigUInt isEqual(const BigUInt& left, const BigUInt& right);

BigUInt isGreater(const BigUInt& left, const BigUInt& right);

BigUInt isLower(const BigUInt& left, const BigUInt& right);

BigUInt isGreaterOrEqual(const BigUInt& left, const BigUInt& right);

BigUInt isLowerOrEqual(const BigUInt& left, const BigUInt& right);

}  // namespace big_uint
