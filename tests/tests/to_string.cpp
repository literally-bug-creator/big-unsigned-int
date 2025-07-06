#include <string>

#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using std::string;
using std::to_string;

constexpr uint64_t MAX_VALUE = 9999999999999999999ULL;
constexpr uint16_t MAX_VALUE_LENGTH = 19;

using namespace big_uint;
namespace {
string makeZerosString(size_t count) {
    return string(count, '0');  // NOLINT
}
}  // namespace

TEST(BigUIntToString, EmptyNumber) {
    BigUInt num = createTestBigUInt({});
    string expected = "0";

    string result = toString(num);

    EXPECT_EQ(result, expected);
}

TEST(BigUIntToString, SingleLimbSmallNumber) {
    BigUInt num = createTestBigUInt({42});
    string expected = "42";

    string result = toString(num);

    EXPECT_EQ(result, expected);
}

TEST(BigUIntToString, SingleLimbMaxValue) {
    BigUInt num = createTestBigUInt({MAX_VALUE});
    string expected = to_string(MAX_VALUE);

    string result = toString(num);

    EXPECT_EQ(result, expected);
}

TEST(BigUIntToString, TwoLimbNumber) {
    BigUInt num = createTestBigUInt({42, 1});
    string padded42 = makeZerosString(MAX_VALUE_LENGTH - to_string(42).size()) + to_string(42);
    string expected = to_string(1) + padded42;

    string result = toString(num);

    EXPECT_EQ(result, expected);
}

TEST(BigUIntToString, ThreeLimbNumber) {
    BigUInt num = createTestBigUInt({123, 4567, 89});
    string padded4567 =
        makeZerosString(MAX_VALUE_LENGTH - to_string(4567).size()) + to_string(4567);
    string padded123 = makeZerosString(MAX_VALUE_LENGTH - to_string(123).size()) + to_string(123);
    string expected = to_string(89) + padded4567 + padded123;

    string result = toString(num);

    EXPECT_EQ(result, expected);
}

TEST(BigUIntToString, MostSignificantNotPadded) {
    BigUInt num = createTestBigUInt({34, 12});
    string padded34 = makeZerosString(MAX_VALUE_LENGTH - to_string(34).size()) + to_string(34);
    string expected = to_string(12) + padded34;

    string result = toString(num);

    EXPECT_EQ(result, expected);
}
