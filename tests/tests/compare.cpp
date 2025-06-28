#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using namespace big_uint;

class BigUIntCompare : public ::testing::Test {};

TEST_F(BigUIntCompare, EqualsEmpty) {
    BigUInt lhs = createTestBigUInt();
    BigUInt rhs = createTestBigUInt();

    EXPECT_TRUE(isEqual(lhs, rhs));
}

TEST_F(BigUIntCompare, EqualsSameLength) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({1});

    EXPECT_TRUE(isEqual(lhs, rhs));
}

TEST_F(BigUIntCompare, NotEqualsByLength) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({1});

    EXPECT_FALSE(isEqual(lhs, rhs));
}

TEST_F(BigUIntCompare, NotEqualsByValue) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({3, 2, 1});

    EXPECT_FALSE(isEqual(lhs, rhs));
}

TEST_F(BigUIntCompare, GreaterByLength) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({1});

    EXPECT_TRUE(isGreater(lhs, rhs));
}

TEST_F(BigUIntCompare, GreaterByValue) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({3, 2, 1});

    EXPECT_TRUE(isGreater(lhs, rhs));
}

TEST_F(BigUIntCompare, LowerByLength) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({1, 2, 3});

    EXPECT_TRUE(isLower(lhs, rhs));
}

TEST_F(BigUIntCompare, LowerByValue) {
    BigUInt lhs = createTestBigUInt({3, 2, 1});
    BigUInt rhs = createTestBigUInt({1, 2, 3});

    EXPECT_TRUE(isLower(lhs, rhs));
}

TEST_F(BigUIntCompare, GreaterOrEqualByLength) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({1, 2, 3});
    BigUInt lhs2 = createTestBigUInt({1, 2, 3});
    BigUInt rhs2 = createTestBigUInt({1, 2});

    EXPECT_TRUE(isGreaterOrEqual(lhs, rhs));
    EXPECT_TRUE(isGreaterOrEqual(lhs2, rhs2));
}

TEST_F(BigUIntCompare, GreaterOrEqualByValue) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt lhs2 = createTestBigUInt({1, 2, 3});
    BigUInt rhs2 = createTestBigUInt({3, 2, 1});

    EXPECT_TRUE(isGreaterOrEqual(lhs, rhs));
    EXPECT_TRUE(isGreaterOrEqual(lhs2, rhs2));
}

TEST_F(BigUIntCompare, LowerOrEqualByLength) {
    BigUInt lhs = createTestBigUInt({1, 2, 3});
    BigUInt rhs = createTestBigUInt({1, 2, 3});
    BigUInt lhs2 = createTestBigUInt({1, 2});
    BigUInt rhs2 = createTestBigUInt({1, 2, 3});

    EXPECT_TRUE(isLowerOrEqual(lhs, rhs));
    EXPECT_TRUE(isLowerOrEqual(lhs2, rhs2));
}

TEST_F(BigUIntCompare, LowerOrEqualByValue) {
    BigUInt lhs = createTestBigUInt({1});
    BigUInt rhs = createTestBigUInt({1});
    BigUInt lhs2 = createTestBigUInt({3, 2, 1});
    BigUInt rhs2 = createTestBigUInt({1, 2, 3});

    EXPECT_TRUE(isLowerOrEqual(lhs, rhs));
    EXPECT_TRUE(isLowerOrEqual(lhs2, rhs2));
}
