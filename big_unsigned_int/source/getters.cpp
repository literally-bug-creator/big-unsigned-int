#include "getters.hpp"

namespace big_uint {
constexpr Chunk ZERO = 0;

size_t getLength(const BigUInt& number) {
    return number.limbs.size();
}

Chunk getChunk(const BigUInt& number, size_t index) {
    if (index >= number.limbs.size()) {
        return ZERO;
    }
    return number.limbs[index];
}

const std::vector<Chunk>& getChunks(const BigUInt& number) {
    return number.limbs;
}
}  // namespace big_uint
