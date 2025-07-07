#include <cstdint>

#include "big_uint.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
std::vector<Chunk> removeLeadingZeros(const std::vector<Chunk>& limbs) {
    auto firstNonZero = static_cast<int64_t>(limbs.size());

    for (size_t index = ZERO; index < limbs.size(); index++) {
        if (limbs[index] != ZERO) {
            firstNonZero = static_cast<int64_t>(index);
            break;
        }
    }

    return {limbs.begin() + firstNonZero, limbs.end()};
}
}  // namespace
BigUInt trim(const BigUInt& number) {
    const std::vector<Chunk>& chunks = getLimbs(number);
    return BigUInt(removeLeadingZeros(chunks));
}
}  // namespace big_uint
