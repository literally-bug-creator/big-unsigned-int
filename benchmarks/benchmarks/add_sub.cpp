#include <cstdint>

#include <benchmark/benchmark.h>
#include <big_uint.hpp>

#include "tools.hpp"

using namespace big_uint;
namespace {
void benchAdd(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    std::vector<Chunk> limbs(range, INT64_MAX);

    BigUInt lhs = createTestBigUInt(limbs);
    BigUInt rhs = createTestBigUInt(limbs);

    for (auto iter : state) {
        add(lhs, rhs);
    }
}

void benchSub(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    std::vector<Chunk> limbs(range, INT64_MAX);

    BigUInt lhs = createTestBigUInt(limbs);
    BigUInt rhs = createTestBigUInt(limbs);

    for (auto iter : state) {
        sub(lhs, rhs);
    }
}
}  // namespace
constexpr size_t MAX_SIZE = 5556;
BENCHMARK(benchAdd)->Range(1, MAX_SIZE);
BENCHMARK(benchSub)->Range(1, MAX_SIZE);
