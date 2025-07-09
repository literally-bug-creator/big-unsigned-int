#include <cstdint>

#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
std::vector<Chunk> removeLeadingZeros(const std::vector<Chunk>& limbs) {
    auto firstNonZero = static_cast<int64_t>(limbs.size());

    for (size_t index = 0; index < limbs.size(); index++) {
        if (limbs[index] != 0) {
            firstNonZero = static_cast<int64_t>(index);
            break;
        }
    }

    return {limbs.begin() + firstNonZero, limbs.end()};
}
}  // namespace
BigUInt trim(const BigUInt& number) noexcept {
    const std::vector<Chunk>& chunks = getLimbs(number);
    return BigUInt(removeLeadingZeros(chunks));
}
}  // namespace big_uint
