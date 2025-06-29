#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using namespace big_uint;

class BigUIntTrim : public ::testing::Test {};

TEST_F(BigUIntTrim, EmptyVector_ShouldReturnZero) {
    BigUInt input = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, SingleZero_ShouldReturnZero) {
    BigUInt input = createTestBigUInt({0});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, MultipleZeros_ShouldReturnSingleZero) {
    BigUInt input = createTestBigUInt({0, 0, 0, 0});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, SingleNonZero_ShouldReturnSame) {
    BigUInt input = createTestBigUInt({42});
    BigUInt expected = createTestBigUInt({42});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, NoLeadingZeros_ShouldReturnSame) {
    BigUInt input = createTestBigUInt({123, 456, 789});
    BigUInt expected = createTestBigUInt({123, 456, 789});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, OneLeadingZero_ShouldTrimIt) {
    BigUInt input = createTestBigUInt({0, 123, 456});
    BigUInt expected = createTestBigUInt({123, 456});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, MultipleLeadingZeros_ShouldTrimAll) {
    BigUInt input = createTestBigUInt({0, 0, 0, 123, 456});
    BigUInt expected = createTestBigUInt({123, 456});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, LeadingZerosWithIntermediateZeros_ShouldTrimOnlyLeading) {
    BigUInt input = createTestBigUInt({0, 0, 0, 123, 0, 456});
    BigUInt expected = createTestBigUInt({123, 0, 456});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, MaxValue_ShouldReturnSame) {
    BigUInt input = createTestBigUInt({UINT64_MAX, UINT64_MAX, UINT64_MAX});
    BigUInt expected = createTestBigUInt({UINT64_MAX, UINT64_MAX, UINT64_MAX});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, MaxValueWithLeadingZeros_ShouldTrimZeros) {
    BigUInt input = createTestBigUInt({0, 0, UINT64_MAX, UINT64_MAX});
    BigUInt expected = createTestBigUInt({UINT64_MAX, UINT64_MAX});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntTrim, SingleNonZeroAtEnd_ShouldKeepOnlyThatElement) {
    BigUInt input = createTestBigUInt({0, 0, 0, 42});
    BigUInt expected = createTestBigUInt({42});

    BigUInt result = trim(input);

    EXPECT_TRUE(isEqual(result, expected));
}
