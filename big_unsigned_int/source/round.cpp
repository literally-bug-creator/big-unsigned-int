#include <cstdint>

#include "big_uint.hpp"
#include "getters.hpp"

namespace big_uint {
namespace {
std::vector<Chunk> removeLeadingChunks(const std::vector<Chunk>& chunks, size_t amount) {
    if (amount == 0) {
        return chunks;
    }
    if (amount >= chunks.size()) {
        return {};
    }
    return {chunks.begin() + static_cast<int64_t>(amount), chunks.end()};
}
}  // namespace
BigUInt round(const BigUInt& number, size_t newSize) {
    size_t numberLength = getLength(number);
    if (newSize >= numberLength) {
        return number;
    }
    const std::vector<Chunk>& chunks = getChunks(number);
    size_t toDelete = numberLength - newSize;
    return BigUInt(removeLeadingChunks(chunks, toDelete));
}
}  // namespace big_uint
