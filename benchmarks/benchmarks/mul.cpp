#include <cstdint>

#include <benchmark/benchmark.h>
#include <big_uint.hpp>

#include "tools.hpp"

using namespace big_uint;
namespace {
void benchMul(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    std::vector<Chunk> limbs(range, INT64_MAX);

    BigUInt lhs = createTestBigUInt(limbs);
    BigUInt rhs = createTestBigUInt(limbs);

    for (auto iter : state) {
        mul(lhs, rhs);
    }
}
}  // namespace
constexpr size_t MAX_SIZE = 5556;
BENCHMARK(benchMul)->Range(1, MAX_SIZE);  // NOLINT(cert-err58-cpp)
