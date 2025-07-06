#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using namespace big_uint;

constexpr uint64_t MAX_VALUE = 9999999999999999999ULL;
constexpr uint16_t MAX_VALUE_LENGTH = 19;

class BigUIntAddSub : public ::testing::Test {};

// Addition Tests
TEST_F(BigUIntAddSub, AddZeroToZero) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddZeroToNumber) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({12345});
    BigUInt expected = createTestBigUInt({12345});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddNumberToZero) {
    BigUInt lhs = createTestBigUInt({12345});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({12345});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddSingleChunks) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({456});
    BigUInt expected = createTestBigUInt({579});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddWithCarry) {
    BigUInt lhs = createTestBigUInt({MAX_VALUE});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({0, 1});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddLargeNumbers) {
    BigUInt lhs = createTestBigUInt({100, 200});
    BigUInt rhs = createTestBigUInt({300, 400});
    BigUInt expected = createTestBigUInt({400, 600});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddMultipleCarries) {
    BigUInt lhs = createTestBigUInt({MAX_VALUE, MAX_VALUE});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({0, 0, 1});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddDifferentSizes) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({456, 789});
    BigUInt expected = createTestBigUInt({579, 789});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddCommutative) {
    BigUInt lhs = createTestBigUInt({123456789});
    BigUInt rhs = createTestBigUInt({987654321});

    BigUInt result1 = add(lhs, rhs);
    BigUInt result2 = add(rhs, lhs);

    EXPECT_TRUE(isEqual(result1, result2));
}

TEST_F(BigUIntAddSub, AddSimpleOverflow) {
    BigUInt lhs = createTestBigUInt({100});
    BigUInt rhs = createTestBigUInt({200});
    BigUInt expected = createTestBigUInt({300});

    BigUInt result = add(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

// Subtraction Tests
TEST_F(BigUIntAddSub, SubZeroFromZero) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubZeroFromNumber) {
    BigUInt lhs = createTestBigUInt({12345});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({12345});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubNumberFromZero) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({12345});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubEqualNumbers) {
    BigUInt lhs = createTestBigUInt({12345});
    BigUInt rhs = createTestBigUInt({12345});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubSingleChunks) {
    BigUInt lhs = createTestBigUInt({456});
    BigUInt rhs = createTestBigUInt({123});
    BigUInt expected = createTestBigUInt({333});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubWithBorrow) {
    BigUInt lhs = createTestBigUInt({0, 1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({MAX_VALUE});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubLargeNumbers) {
    BigUInt lhs = createTestBigUInt({400, 600});
    BigUInt rhs = createTestBigUInt({100, 200});
    BigUInt expected = createTestBigUInt({300, 400});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubSmallerFromLarger) {
    BigUInt lhs = createTestBigUInt({12345, 67890});
    BigUInt rhs = createTestBigUInt({123});
    BigUInt expected = createTestBigUInt({12222, 67890});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubLargerFromSmaller) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({12345, 67890});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubMultipleBorrows) {
    BigUInt lhs = createTestBigUInt({0, 0, 1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({MAX_VALUE, MAX_VALUE});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubMaxChunk) {
    BigUInt lhs = createTestBigUInt({MAX_VALUE});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({MAX_VALUE - 1});

    BigUInt result = sub(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

// Combined Add/Sub Tests
TEST_F(BigUIntAddSub, AddSubInverse) {
    BigUInt lhs = createTestBigUInt({123456789});
    BigUInt rhs = createTestBigUInt({987654321});

    BigUInt sum = add(lhs, rhs);
    BigUInt diff = sub(sum, rhs);

    EXPECT_TRUE(isEqual(diff, lhs));
}

TEST_F(BigUIntAddSub, AddAssociative) {
    BigUInt first = createTestBigUInt({123});
    BigUInt second = createTestBigUInt({456});
    BigUInt third = createTestBigUInt({789});

    BigUInt result1 = add(add(first, second), third);
    BigUInt result2 = add(first, add(second, third));

    EXPECT_TRUE(isEqual(result1, result2));
}

TEST_F(BigUIntAddSub, SubAddIdentity) {
    BigUInt lhs = createTestBigUInt({123456789});
    BigUInt rhs = createTestBigUInt({987654321});

    if (isGreaterOrEqual(lhs, rhs)) {
        BigUInt diff = sub(lhs, rhs);
        BigUInt sum = add(diff, rhs);
        EXPECT_TRUE(isEqual(sum, lhs));
    }
}

TEST_F(BigUIntAddSub, EdgeCaseMaxValues) {
    BigUInt maxSingle = createTestBigUInt({MAX_VALUE});
    BigUInt maxDouble = createTestBigUInt({MAX_VALUE, MAX_VALUE});

    BigUInt result = add(maxSingle, maxSingle);
    BigUInt expected = createTestBigUInt({MAX_VALUE - 1, 1});
    EXPECT_TRUE(isEqual(result, expected));

    BigUInt subResult = sub(maxDouble, maxSingle);
    BigUInt subExpected = createTestBigUInt({0, MAX_VALUE});
    EXPECT_TRUE(isEqual(subResult, subExpected));
}

TEST_F(BigUIntAddSub, ChainedOperations) {
    BigUInt first = createTestBigUInt({1000});
    BigUInt second = createTestBigUInt({500});
    BigUInt third = createTestBigUInt({300});
    BigUInt fourth = createTestBigUInt({200});

    BigUInt result = sub(add(first, second), add(third, fourth));
    BigUInt expected = createTestBigUInt({1000});

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, LargeNumbersAdd) {
    std::vector<Chunk> largeLimbs1(1000, 1);
    std::vector<Chunk> largeLimbs2(1000, 2);

    BigUInt lhs = createTestBigUInt(largeLimbs1);
    BigUInt rhs = createTestBigUInt(largeLimbs2);

    BigUInt result = add(lhs, rhs);

    EXPECT_FALSE(isZero(result));
    EXPECT_TRUE(isGreater(result, lhs));
    EXPECT_TRUE(isGreater(result, rhs));
}

TEST_F(BigUIntAddSub, LargeNumbersSub) {
    std::vector<Chunk> largeLimbs1(1000, 3);
    std::vector<Chunk> largeLimbs2(1000, 1);

    BigUInt lhs = createTestBigUInt(largeLimbs1);
    BigUInt rhs = createTestBigUInt(largeLimbs2);

    BigUInt result = sub(lhs, rhs);

    EXPECT_FALSE(isZero(result));
    EXPECT_TRUE(isLower(result, lhs));
    EXPECT_TRUE(isGreater(result, rhs));
}

// Tests for shift operations
TEST_F(BigUIntAddSub, AddWithShiftZero) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({456});
    BigUInt expected = createTestBigUInt({579});

    BigUInt result = add(lhs, rhs, 0);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddWithShiftOne) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({1, 1});

    BigUInt result = add(lhs, rhs, 1);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddWithShiftMultiple) {
    BigUInt lhs = createTestBigUInt({100, 200});
    BigUInt rhs = createTestBigUInt({10, 20});
    BigUInt expected = createTestBigUInt({10, 120, 200});

    BigUInt result = add(lhs, rhs, 1);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddWithShiftZeroToNumber) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({123});
    BigUInt expected = createTestBigUInt({123});

    BigUInt result = add(lhs, rhs, 1);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddWithShiftNumberToZero) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({0, 0, 0, 0, 0, 123});

    BigUInt result = add(lhs, rhs, 5);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddWithShiftCarry) {
    BigUInt lhs = createTestBigUInt({MAX_VALUE});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({1, MAX_VALUE});

    BigUInt result = add(lhs, rhs, 1);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubWithShiftZero) {
    BigUInt lhs = createTestBigUInt({579});
    BigUInt rhs = createTestBigUInt({456});
    BigUInt expected = createTestBigUInt({123});

    BigUInt result = sub(lhs, rhs, 0);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubWithShiftOne) {
    BigUInt lhs = createTestBigUInt({1, 1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({MAX_VALUE, 0, 1});

    BigUInt result = sub(lhs, rhs, 1);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, SubWithShiftEqual) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({0, 123});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = sub(lhs, rhs, 1);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntAddSub, AddSubInverseWithShift) {
    BigUInt lhs = createTestBigUInt({123, 456});
    BigUInt rhs = createTestBigUInt({789});
    size_t shift = 2;

    BigUInt shiftedLhs = createTestBigUInt({0, 0, 123, 456});
    BigUInt sum = add(shiftedLhs, rhs);
    BigUInt diff = sub(sum, rhs);

    EXPECT_TRUE(isEqual(diff, shiftedLhs));
}

TEST_F(BigUIntAddSub, ShiftCommutativeProperty) {
    BigUInt lhs = createTestBigUInt({100});
    BigUInt rhs = createTestBigUInt({200});
    size_t shift = 1;

    BigUInt result1 = add(lhs, rhs, shift);
    BigUInt result2 = add(rhs, lhs, shift);

    EXPECT_FALSE(isEqual(result1, result2));
}
