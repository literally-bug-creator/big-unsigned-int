#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "big_uint.hpp"
#include "tools.hpp"

using namespace big_uint;

using std::string;
using std::vector;

namespace {
vector<Digit> stringToDigits(const string& str) {
    vector<Digit> digits;
    for (char letter : str) {
        if (letter < '0' || letter > '9') {
            throw std::invalid_argument("invalid digit");
        }
        digits.push_back(static_cast<Digit>(letter - '0'));
    }
    return digits;
}
}  // namespace

TEST(BigUIntConstructors, EmptyVector) {
    vector<Digit> digits = {};
    BigUInt expected = makeZero();

    BigUInt result = makeBigUInt(digits);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST(BigUIntConstructors, SingleDigit) {
    vector<Digit> digits = {7};
    BigUInt expected = createTestBigUInt({7});

    BigUInt result = makeBigUInt(digits);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST(BigUIntConstructors, MultiDigitLessThanChunkSize) {
    vector<Digit> digits = stringToDigits("1234");
    BigUInt expected = createTestBigUInt({1234});

    BigUInt result = makeBigUInt(digits);

    EXPECT_TRUE(isEqual(result, expected));
}

TEST(BigUIntConstructors, ExactlyOneChunk) {
    string numStr(MAX_VALUE_LENGTH, '1');
    vector<Digit> digits = stringToDigits(numStr);
    Chunk limbValue = std::stoull(numStr);
    BigUInt expected = createTestBigUInt({limbValue});

    BigUInt result = makeBigUInt(digits);

    EXPECT_TRUE(isEqual(result, expected));
}

// TEST(BigUIntConstructors, OneMoreThanOneChunk) {
//     vector<Digit> digits = stringToDigits(numStr);
//     Chunk limb0 = std::stoull(numStr.substr(1));
//     Chunk limb1 = std::stoull(numStr.substr(0, 1));
//     BigUInt expected = createTestBigUInt({limb0, limb1});

//     BigUInt result = makeBigUInt(digits);

//     EXPECT_TRUE(isEqual(result, expected));
// }
