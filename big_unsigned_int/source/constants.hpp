#include <cstdint>
#include <string>

#include "big_uint.hpp"

namespace big_uint {
constexpr uint64_t MAX_VALUE = 9999999999999999999ULL;
constexpr uint16_t MAX_VALUE_LENGTH = 19;

constexpr Chunk ZERO_CHUNK = 0;

constexpr uint8_t ZERO = 0;
constexpr uint8_t ONE = 1;
constexpr uint8_t TWO = 2;
constexpr int16_t TEN = 10;

constexpr std::string ONE_STR = "1";
constexpr std::string ZERO_STR = "0";

constexpr char ZERO_CHAR = '0';
}  // namespace big_uint
