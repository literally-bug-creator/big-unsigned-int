#include <cstdint>

#include <benchmark/benchmark.h>
#include <big_uint.hpp>

#include "tools.hpp"

using namespace big_uint;
namespace {
void benchToString(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    std::vector<Chunk> limbs(range, INT64_MAX);

    BigUInt number = createTestBigUInt(limbs);

    for (auto iter : state) {
        toString(number);
    }
}
}  // namespace
constexpr size_t MAX_SIZE = 5556;
BENCHMARK(benchToString)->Range(1, MAX_SIZE);  // NOLINT(cert-err58-cpp)
