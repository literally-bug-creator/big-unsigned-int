#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace big_uint {
using std::string;

using Chunk = uint64_t;
using Digit = uint8_t;

constexpr uint64_t MAX_VALUE = 9999999999999999999ULL;
constexpr uint16_t MAX_VALUE_LENGTH = 19;

struct BigUInt {
    std::vector<Chunk> limbs;
};

BigUInt makeBigUInt(const std::vector<Digit>& digits);

BigUInt makeZero();

BigUInt add(const BigUInt& augend, const BigUInt& addend);

BigUInt add(const BigUInt& augend, const BigUInt& addend, size_t shift);

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend);

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend, size_t shift);

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier);

size_t getSize(const BigUInt& number);

string toString(const BigUInt& number);

bool isZero(const BigUInt& number);

bool isEqual(const BigUInt& left, const BigUInt& right);

bool isGreater(const BigUInt& left, const BigUInt& right);

bool isLower(const BigUInt& left, const BigUInt& right);

bool isGreaterOrEqual(const BigUInt& left, const BigUInt& right);

bool isLowerOrEqual(const BigUInt& left, const BigUInt& right);

BigUInt trim(const BigUInt& number);

BigUInt round(const BigUInt& number, size_t newSize);

}  // namespace big_uint
