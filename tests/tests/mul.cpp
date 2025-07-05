#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using namespace big_uint;

class BigUIntMul : public ::testing::Test {};

TEST_F(BigUIntMul, ZeroByZero) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, ZeroByNumber) {
    BigUInt lhs = createTestBigUInt({});
    BigUInt rhs = createTestBigUInt({42});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, NumberByZero) {
    BigUInt lhs = createTestBigUInt({42});
    BigUInt rhs = createTestBigUInt({});
    BigUInt expected = createTestBigUInt({});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, SingleChunkByOne) {
    BigUInt lhs = createTestBigUInt({42});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({42});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, OneByOne) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({1});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, TwoByThree) {
    BigUInt lhs = createTestBigUInt({2});
    BigUInt rhs = createTestBigUInt({3});
    BigUInt expected = createTestBigUInt({6});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, SingleChunkByTwo) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({2});
    BigUInt expected = createTestBigUInt({246});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, MaxChunkByTwo) {
    BigUInt lhs = createTestBigUInt({UINT64_MAX});
    BigUInt rhs = createTestBigUInt({2});
    BigUInt expected = createTestBigUInt({UINT64_MAX - 1, 1});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, MaxChunkByMaxChunk) {
    BigUInt lhs = createTestBigUInt({UINT64_MAX});
    BigUInt rhs = createTestBigUInt({UINT64_MAX});
    BigUInt expected = createTestBigUInt({1, UINT64_MAX - 1});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, TwoChunksByOne) {
    BigUInt lhs = createTestBigUInt({100, 200});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({100, 200});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, OneByTwoChunks) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({100, 200});
    BigUInt expected = createTestBigUInt({100, 200});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, TwoChunksByTwo) {
    BigUInt lhs = createTestBigUInt({100, 200});
    BigUInt rhs = createTestBigUInt({2});
    BigUInt expected = createTestBigUInt({200, 400});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, SmallMultiplication) {
    BigUInt lhs = createTestBigUInt({123});
    BigUInt rhs = createTestBigUInt({456});
    BigUInt expected = createTestBigUInt({56088});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, TwoChunksByTwoChunks) {
    BigUInt lhs = createTestBigUInt({1, 2});
    BigUInt rhs = createTestBigUInt({3, 4});
    BigUInt expected = createTestBigUInt({3, 10, 8});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, ThreeChunksByOne) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({10});
    BigUInt expected = createTestBigUInt({10, 20, 30});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, OneByThreeChunks) {
    BigUInt lhs = createTestBigUInt({5});
    BigUInt rhs = createTestBigUInt({2, 4, 6});
    BigUInt expected = createTestBigUInt({10, 20, 30});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, WithCarry) {
    BigUInt lhs = createTestBigUInt({UINT64_MAX / 2});
    BigUInt rhs = createTestBigUInt({4});
    BigUInt expected = createTestBigUInt({((UINT64_MAX / 2) * 4) % UINT64_MAX, 1});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, LargeNumbers) {
    BigUInt lhs = createTestBigUInt({UINT64_MAX, UINT64_MAX});
    BigUInt rhs = createTestBigUInt({2});
    BigUInt expected = createTestBigUInt({UINT64_MAX - 1, UINT64_MAX, 1});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, Commutative) {
    BigUInt lhs = createTestBigUInt({123, 456});
    BigUInt rhs = createTestBigUInt({789});

    BigUInt result1 = mul(lhs, rhs);
    BigUInt result2 = mul(rhs, lhs);

    EXPECT_TRUE(isEqual(result1, result2));
}

TEST_F(BigUIntMul, Power) {
    BigUInt lhs = createTestBigUInt({2});
    BigUInt rhs = createTestBigUInt({2});
    BigUInt expected = createTestBigUInt({4});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, PowerOfTwo) {
    BigUInt lhs = createTestBigUInt({16});
    BigUInt rhs = createTestBigUInt({16});
    BigUInt expected = createTestBigUInt({256});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, ManyChunks) {
    BigUInt lhs = createTestBigUInt({1, 1, 1, 1, 1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt expected = createTestBigUInt({1, 1, 1, 1, 1});

    BigUInt result = mul(lhs, rhs);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST_F(BigUIntMul, Associative) {
    BigUInt first = createTestBigUInt({2});
    BigUInt second = createTestBigUInt({3});
    BigUInt third = createTestBigUInt({4});

    BigUInt result1 = mul(mul(first, second), third);
    BigUInt result2 = mul(first, mul(second, third));

    EXPECT_TRUE(isEqual(result1, result2));
}

TEST_F(BigUIntMul, OverflowHandling) {
    BigUInt lhs = createTestBigUInt({UINT64_MAX, UINT64_MAX, UINT64_MAX});
    BigUInt rhs = createTestBigUInt({UINT64_MAX});

    BigUInt result = mul(lhs, rhs);

    EXPECT_FALSE(isZero(result));
    EXPECT_TRUE(isGreater(result, lhs));
}
