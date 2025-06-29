#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using namespace big_uint;

class BigUIntRound : public ::testing::Test {};

TEST_F(BigUIntRound, NewSizeEqualToLength_ShouldReturnSame) {
    BigUInt input = createTestBigUInt({123, 456, 789});
    size_t newSize = 3;
    BigUInt expected = createTestBigUInt({123, 456, 789});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, NewSizeGreaterThanLength_ShouldReturnSame) {
    BigUInt input = createTestBigUInt({123, 456});
    size_t newSize = 5;
    BigUInt expected = createTestBigUInt({123, 456});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, NewSizeZero_ShouldReturnEmpty) {
    BigUInt input = createTestBigUInt({123, 456, 789});
    size_t newSize = 0;
    BigUInt expected = createTestBigUInt({});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, NewSizeOne_ShouldKeepLastChunk) {
    BigUInt input = createTestBigUInt({123, 456, 789});
    size_t newSize = 1;
    BigUInt expected = createTestBigUInt({789});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, NewSizeTwo_ShouldKeepLastTwoChunks) {
    BigUInt input = createTestBigUInt({123, 456, 789});
    size_t newSize = 2;
    BigUInt expected = createTestBigUInt({456, 789});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, SingleChunk_NewSizeZero_ShouldReturnEmpty) {
    BigUInt input = createTestBigUInt({42});
    size_t newSize = 0;
    BigUInt expected = createTestBigUInt({});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, SingleChunk_NewSizeOne_ShouldReturnSame) {
    BigUInt input = createTestBigUInt({42});
    size_t newSize = 1;
    BigUInt expected = createTestBigUInt({42});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, EmptyInput_NewSizeZero_ShouldReturnEmpty) {
    BigUInt input = createTestBigUInt({});
    size_t newSize = 0;
    BigUInt expected = createTestBigUInt({});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, EmptyInput_NewSizePositive_ShouldReturnEmpty) {
    BigUInt input = createTestBigUInt({});
    size_t newSize = 3;
    BigUInt expected = createTestBigUInt({});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, LargeNumber_RoundToMiddle) {
    BigUInt input = createTestBigUInt({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    size_t newSize = 5;
    BigUInt expected = createTestBigUInt({6, 7, 8, 9, 10});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, WithZeros_ShouldPreserveZeros) {
    BigUInt input = createTestBigUInt({0, 123, 0, 456, 0});
    size_t newSize = 3;
    BigUInt expected = createTestBigUInt({0, 456, 0});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntRound, AllZeros_NewSizeOne_ShouldReturnSingleZero) {
    BigUInt input = createTestBigUInt({0, 0, 0, 0});
    size_t newSize = 1;
    BigUInt expected = createTestBigUInt({0});

    BigUInt result = round(input, newSize);

    EXPECT_TRUE(isEqual(result, expected));
}
