#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
std::vector<Chunk> removeTrailingZeros(const std::vector<Chunk>& limbs) {
    auto lastNonZero = static_cast<int64_t>(-ONE);
    for (int64_t index = static_cast<int64_t>(limbs.size()) - ONE; index >= ZERO; index--) {
        if (limbs[static_cast<size_t>(index)] != ZERO) {
            lastNonZero = index;
            break;
        }
    }
    return {limbs.begin(), limbs.begin() + lastNonZero + ONE};
}
}  // namespace
BigUInt shiftLeft(const BigUInt& number, size_t shiftBits) {
    size_t limbShift = shiftBits / 64;
    const std::vector<Chunk>& limbs = getLimbs(number);
    std::vector<Chunk> result(limbShift, 0);
    result.insert(result.end(), limbs.begin(), limbs.end());
    return BigUInt{removeTrailingZeros(result)};
}

}  // namespace big_uint
