#include <cstdint>
#include <string>

#include "big_uint.hpp"
#include "constants.hpp"

namespace big_uint {
std::string toString(const BigUInt& number) {
    if (number.limbs.empty()) {
        return ZERO_STR;
    }
    uint64_t carry = 0;
    std::string result;
    result.reserve(getSize(number));
    result += std::to_string(number.limbs.back());
    for (auto it = number.limbs.rbegin() + ONE; it != number.limbs.rend(); ++it) {
        __uint128_t curr = *it + carry;
        carry = curr / MAX_VALUE_TEN;
        uint64_t value = curr % MAX_VALUE_TEN;
        std::string limbStr = std::to_string(value);
        size_t numZeros =
            MAX_VALUE_LENGTH > limbStr.size() ? MAX_VALUE_LENGTH - limbStr.size() : ZERO;
        result += std::string(numZeros, ZERO_CHAR) + limbStr;
    }
    if (carry > 0) {
        result += std::to_string(carry);
    }
    return result;
}
}  // namespace big_uint
