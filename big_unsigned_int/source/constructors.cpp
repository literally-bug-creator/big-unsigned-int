#include <vector>

#include "big_uint.hpp"

namespace big_uint {

BigUInt makeBigUInt(const std::vector<Digit>& digits) noexcept {
    if (digits.empty()) {
        return makeZero();
    }

    std::vector<Chunk> limbs;
    const size_t CHUNK_SIZE = MAX_VALUE_LENGTH;
    size_t total = digits.size();

    while (total > 0) {
        size_t blockSize = (total >= CHUNK_SIZE) ? CHUNK_SIZE : total;
        size_t start = total - blockSize;
        Chunk value = 0;

        for (size_t i = start; i < total; ++i) {
            value = value * 10 + static_cast<Chunk>(digits[i]);
        }

        limbs.push_back(value);
        total -= blockSize;
    }
    return BigUInt{limbs};
}

BigUInt makeZero() noexcept {
    return BigUInt({});
}

}  // namespace big_uint
