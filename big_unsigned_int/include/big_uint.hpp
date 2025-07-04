#pragma once

#include <cstdint>
#include <vector>

namespace big_uint {
using Chunk = uint64_t;

struct BigUInt {
    std::vector<Chunk> limbs;
};

BigUInt makeZero();

BigUInt add(const BigUInt& augend, const BigUInt& addend);

BigUInt add(const BigUInt& augend, const BigUInt& addend, size_t shift);

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend);

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend, size_t shift);

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier);

bool isZero(const BigUInt& number);

bool isEqual(const BigUInt& left, const BigUInt& right);

bool isGreater(const BigUInt& left, const BigUInt& right);

bool isLower(const BigUInt& left, const BigUInt& right);

bool isGreaterOrEqual(const BigUInt& left, const BigUInt& right);

bool isLowerOrEqual(const BigUInt& left, const BigUInt& right);

BigUInt trim(const BigUInt& number);

BigUInt round(const BigUInt& number, size_t newSize);

}  // namespace big_uint
