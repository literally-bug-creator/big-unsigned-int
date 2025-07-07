#include <string>

#include "big_uint.hpp"
#include "constants.hpp"

namespace big_uint {
std::string toString(const BigUInt& number) {
    if (number.limbs.empty()) {
        return ZERO_STR;
    }
    std::string result;
    result.reserve(getSize(number));
    result += std::to_string(number.limbs.back());
    for (auto it = number.limbs.rbegin() + ONE; it != number.limbs.rend(); ++it) {
        std::string limbStr = std::to_string(*it);
        size_t numZeros =
            MAX_VALUE_LENGTH > limbStr.size() ? MAX_VALUE_LENGTH - limbStr.size() : ZERO;
        result += std::string(numZeros, ZERO_CHAR) + limbStr;
    }
    return result;
}
}  // namespace big_uint
