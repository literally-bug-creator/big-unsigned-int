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
constexpr uint64_t MAX_DEGREE_OF_TEN = 1000000000000000000ULL;

struct BigUInt {
    std::vector<Chunk> limbs;
};

BigUInt makeBigUInt(const std::vector<Digit>& digits) noexcept;

BigUInt makeZero() noexcept;

BigUInt add(const BigUInt& augend, const BigUInt& addend) noexcept;

BigUInt add(const BigUInt& augend, const BigUInt& addend, size_t shift) noexcept;

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend) noexcept;

BigUInt sub(const BigUInt& minuend, const BigUInt& subtrahend, size_t shift) noexcept;

BigUInt mul(const BigUInt& multiplicand, const BigUInt& multiplier) noexcept;

size_t getSize(const BigUInt& number) noexcept;

string toString(const BigUInt& number) noexcept;

bool isZero(const BigUInt& number) noexcept;

bool isEqual(const BigUInt& left, const BigUInt& right) noexcept;

bool isGreater(const BigUInt& left, const BigUInt& right) noexcept;

bool isLower(const BigUInt& left, const BigUInt& right) noexcept;

bool isGreaterOrEqual(const BigUInt& left, const BigUInt& right) noexcept;

bool isLowerOrEqual(const BigUInt& left, const BigUInt& right) noexcept;

BigUInt trim(const BigUInt& number) noexcept;

BigUInt round(const BigUInt& number, size_t newSize) noexcept;

}  // namespace big_uint
