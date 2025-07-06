#include <benchmark/benchmark.h>
#include <big_uint.hpp>

using namespace big_uint;

namespace {
void benchConstructor(benchmark::State& state) {
    auto range = static_cast<size_t>(state.range(0));
    std::vector<Digit> digits(range, 9);

    for (auto iter : state) {
        makeBigUInt(digits);
    }
}
}  // namespace
constexpr size_t MAX_SIZE = 100000;
BENCHMARK(benchConstructor)->Range(1, MAX_SIZE);  // NOLINT(cert-err58-cpp)
