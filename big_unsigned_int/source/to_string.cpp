#include <string>

#include "big_uint.hpp"

namespace big_uint {
std::string toString(const BigUInt& number) {
    if (number.limbs.empty()) {
        return "0";
    }
    std::string result;
    result.reserve(getSize(number));
    result += std::to_string(number.limbs.back());
    for (auto it = number.limbs.rbegin() + 1; it != number.limbs.rend(); ++it) {
        std::string limbStr = std::to_string(*it);
        size_t numZeros = MAX_VALUE_LENGTH > limbStr.size() ? MAX_VALUE_LENGTH - limbStr.size() : 0;
        result += std::string(numZeros, '0') + limbStr;
    }
    return result;
}
}  // namespace big_uint
