#include <benchmark/benchmark.h>
#include <big_uint.hpp>

#include "tools.hpp"

using namespace big_uint;
namespace {
void isEqualBench(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    Chunk maxValue = 999999999999999999;
    std::vector<Chunk> lhsChunks(range, maxValue);
    std::vector<Chunk> rhsChunks(range, maxValue);

    BigUInt lhs = createTestBigUInt(lhsChunks);
    BigUInt rhs = createTestBigUInt(rhsChunks);

    for (auto iter : state) {
        isEqual(lhs, rhs);
    }
}

void isGreaterBench(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    Chunk maxValue = 999999999999999999;
    std::vector<Chunk> lhsChunks(range, maxValue);
    std::vector<Chunk> rhsChunks(range, maxValue);

    BigUInt lhs = createTestBigUInt(lhsChunks);
    BigUInt rhs = createTestBigUInt(rhsChunks);

    for (auto iter : state) {
        isGreater(lhs, rhs);
    }
}

void isLowerBench(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    Chunk maxValue = 999999999999999999;
    std::vector<Chunk> lhsChunks(range, maxValue);
    std::vector<Chunk> rhsChunks(range, maxValue);

    BigUInt lhs = createTestBigUInt(lhsChunks);
    BigUInt rhs = createTestBigUInt(rhsChunks);

    for (auto iter : state) {
        isLower(lhs, rhs);
    }
}

void isGreaterOrEqualBench(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    Chunk maxValue = 999999999999999999;
    std::vector<Chunk> lhsChunks(range, maxValue);
    std::vector<Chunk> rhsChunks(range, maxValue);

    BigUInt lhs = createTestBigUInt(lhsChunks);
    BigUInt rhs = createTestBigUInt(rhsChunks);

    for (auto iter : state) {
        isGreaterOrEqual(lhs, rhs);
    }
}

void isLowerOrEqualBench(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    Chunk maxValue = 999999999999999999;
    std::vector<Chunk> lhsChunks(range, maxValue);
    std::vector<Chunk> rhsChunks(range, maxValue);

    BigUInt lhs = createTestBigUInt(lhsChunks);
    BigUInt rhs = createTestBigUInt(rhsChunks);

    for (auto iter : state) {
        isLowerOrEqual(lhs, rhs);
    }
}
}  // namespace
constexpr size_t MAX_SIZE = 5556;
BENCHMARK(isEqualBench)->Range(1, MAX_SIZE);           // NOLINT(cert-err58-cpp)
BENCHMARK(isGreaterBench)->Range(1, MAX_SIZE);         // NOLINT(cert-err58-cpp)
BENCHMARK(isLowerBench)->Range(1, MAX_SIZE);           // NOLINT(cert-err58-cpp)
BENCHMARK(isGreaterOrEqualBench)->Range(1, MAX_SIZE);  // NOLINT(cert-err58-cpp)
BENCHMARK(isLowerOrEqualBench)->Range(1, MAX_SIZE);    // NOLINT(cert-err58-cpp)
