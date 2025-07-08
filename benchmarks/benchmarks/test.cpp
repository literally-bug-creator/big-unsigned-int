#include <cstdint>

#include <benchmark/benchmark.h>
#include <big_uint.hpp>

#include "tools.hpp"

using namespace big_uint;
namespace {
// std::string digitsToStringUniversal(const std::vector<uint64_t>& digits,
//                                     uint64_t base = UINT64_MAX) {
//     if (digits.empty())
//         return "0";

//     std::vector<uint64_t> temp_digits = digits;
//     std::string result;

//     while (!temp_digits.empty()) {
//         uint64_t remainder = 0;

//         for (int i = temp_digits.size() - 1; i >= 0; --i) {
//             __uint128_t current = remainder * base + temp_digits[i];
//             temp_digits[i] = current / 10;
//             remainder = current % 10;
//         }

//         result = char('0' + remainder) + result;

//         while (!temp_digits.empty() && temp_digits.back() == 0) {
//             temp_digits.pop_back();
//         }
//     }

//     return result.empty() ? "0" : result;
// }
//

void benchTestOutput(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    std::vector<uint64_t> limbs(range, UINT64_MAX);

    for (auto iter : state) {
        test(limbs);
    }
}
}  // namespace
constexpr size_t MAX_SIZE = 5555;
BENCHMARK(benchTestOutput)->Range(1, MAX_SIZE);  // NOLINT(cert-err58-cpp)
