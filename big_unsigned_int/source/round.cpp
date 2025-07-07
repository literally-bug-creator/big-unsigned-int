#include <cstdint>

#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
std::vector<Chunk> removeLeadingLimbs(const std::vector<Chunk>& limbs, size_t amount) {
    if (amount == ZERO) {
        return limbs;
    }
    if (amount >= limbs.size()) {
        return {};
    }
    return {limbs.begin() + static_cast<int64_t>(amount), limbs.end()};
}
}  // namespace
BigUInt round(const BigUInt& number, size_t newSize) {
    size_t numberLength = getSize(number);
    if (newSize >= numberLength) {
        return number;
    }
    const std::vector<Chunk>& limbs = getLimbs(number);
    size_t toDelete = numberLength - newSize;
    return BigUInt(removeLeadingLimbs(limbs, toDelete));
}
}  // namespace big_uint
