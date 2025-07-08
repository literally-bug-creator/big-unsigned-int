#include <cstdint>
#include <string>

#include <sys/types.h>

#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
std::string addLeadingZeros(const std::string& numberStr) {
    size_t numZeros = MAX_VALUE_LENGTH > numberStr.size() ? MAX_VALUE_LENGTH - numberStr.size() : 0;
    return std::string(numZeros, '0') + numberStr;
}
}  // namespace

std::string toString(const BigUInt& number) {
    static constexpr std::string ZERO_STR = "0";
    if (isZero(number)) {
        return ZERO_STR;
    }
    std::string result;
    result.reserve(getSize(number));
    std::vector<Chunk> limbs = getLimbs(number);
    uint64_t carry = 0;
    for (size_t index = 0; index < limbs.size() - 1; index++) {
        __uint128_t curr = limbs[index] + carry;
        carry = static_cast<uint64_t>(curr / MAX_DEGREE_OF_TEN);
        auto remainder = static_cast<uint64_t>(curr % MAX_DEGREE_OF_TEN);
        std::string limbStr = addLeadingZeros(std::to_string(remainder));
        result.insert(0, limbStr);
    }
    uint64_t last = number.limbs.back() + carry;
    carry = last / MAX_DEGREE_OF_TEN;
    result = std::to_string(last % MAX_DEGREE_OF_TEN) + result;
    if (carry > 0) {
        result = std::to_string(carry) + result;
    }
    return result;
}
}  // namespace big_uint
