#include "getters.hpp"

namespace big_uint {
size_t getLength(const BigUInt& number) {
    return number.limbs.size();
}
}  // namespace big_uint
