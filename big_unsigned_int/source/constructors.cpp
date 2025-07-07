#include <vector>

#include "big_uint.hpp"
#include "constants.hpp"

namespace big_uint {

BigUInt makeBigUInt(const std::vector<Digit>& digits) {
    if (digits.empty()) {
        return makeZero();
    }

    std::vector<Chunk> limbs;
    const size_t CHUNK_SIZE = MAX_VALUE_LENGTH;
    size_t total = digits.size();

    while (total > ZERO) {
        size_t blockSize = (total >= CHUNK_SIZE) ? CHUNK_SIZE : total;
        size_t start = total - blockSize;
        Chunk value = ZERO;

        for (size_t i = start; i < total; ++i) {
            value = value * TEN + static_cast<Chunk>(digits[i]);
        }

        limbs.push_back(value);
        total -= blockSize;
    }
    return BigUInt{limbs};
}

BigUInt makeZero() {
    return BigUInt({});
}

}  // namespace big_uint
