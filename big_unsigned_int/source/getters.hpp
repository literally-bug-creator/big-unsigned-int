#pragma once

#include "big_uint.hpp"

namespace big_uint {
size_t getLength(const BigUInt& number);

Chunk getChunk(const BigUInt& number, size_t index);

const std::vector<Chunk>& getLimbs(const BigUInt& number);
}  // namespace big_uint
