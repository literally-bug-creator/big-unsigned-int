#include <algorithm>
#include <benchmark/benchmark.h>
#include <cmath>
#include <numeric>
#include <random>
#include <vector>

// Matrix multiplication benchmark - computationally intensive
static void BM_MatrixMultiplication(benchmark::State& state) {
    const size_t size = static_cast<size_t>(state.range(0));
    std::vector<std::vector<double>> A(size, std::vector<double>(size));
    std::vector<std::vector<double>> B(size, std::vector<double>(size));
    std::vector<std::vector<double>> C(size, std::vector<double>(size));

  // Initialize matrices with random values
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(1.0, 100.0);

  for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
          A[i][j] = dis(gen);
          B[i][j] = dis(gen);
      }
  }

  for (auto _ : state) {
      // Matrix multiplication C = A * B
      for (size_t i = 0; i < size; ++i) {
          for (size_t j = 0; j < size; ++j) {
              C[i][j] = 0.0;
              for (size_t k = 0; k < size; ++k) {
                  C[i][j] += A[i][k] * B[k][j];
              }
          }
      }
      benchmark::DoNotOptimize(C);
  }

  state.SetComplexityN(static_cast<int64_t>(size));
}
BENCHMARK(BM_MatrixMultiplication)->Range(32, 256)->Complexity(benchmark::oNCubed);

// Vector operations benchmark - shows vectorization benefits
static void BM_VectorOperations(benchmark::State& state) {
    const size_t size = static_cast<size_t>(state.range(0));
    std::vector<double> a(size), b(size), c(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-100.0, 100.0);

    std::generate(a.begin(), a.end(), [&]() { return dis(gen); });
    std::generate(b.begin(), b.end(), [&]() { return dis(gen); });

    for (auto _ : state) {
        // Vector arithmetic operations
        for (size_t i = 0; i < size; ++i) {
            c[i] = std::sqrt(a[i] * a[i] + b[i] * b[i]) * std::sin(a[i]) + std::cos(b[i]);
        }
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(size) * static_cast<int64_t>(sizeof(double)) * 3);
}
BENCHMARK(BM_VectorOperations)->Range(1024, 1024 * 1024)->UseRealTime();

// Recursive Fibonacci - shows optimization of function calls
static long long fibonacci(int n) {
  if (n <= 1)
    return n;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

static void BM_RecursiveFibonacci(benchmark::State& state) {
    const int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        long long result = fibonacci(n);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_RecursiveFibonacci)->Range(25, 40);

// Memory allocation and deallocation
static void BM_MemoryOperations(benchmark::State& state) {
    const size_t count = static_cast<size_t>(state.range(0));
    const size_t size = 1024; // 1KB per allocation

    for (auto _ : state) {
        std::vector<std::unique_ptr<char[]>> ptrs;
        ptrs.reserve(count);

        // Allocate
        for (size_t i = 0; i < count; ++i) {
            ptrs.emplace_back(std::make_unique<char[]>(size));
            // Write to memory to prevent optimization
            std::fill_n(ptrs.back().get(), size, static_cast<char>(i % 256));
        }

        // Access pattern that prevents optimization
        size_t sum = 0;
        for (const auto& ptr : ptrs) {
            sum += static_cast<size_t>(ptr[0]);
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_MemoryOperations)->Range(100, 10000);

// Sorting algorithm - shows loop optimization benefits
static void BM_SortingOperations(benchmark::State& state) {
    const size_t size = static_cast<size_t>(state.range(0));

    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto _ : state) {
        state.PauseTiming();
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 0);
        std::shuffle(data.begin(), data.end(), gen);
        state.ResumeTiming();

        std::sort(data.begin(), data.end());
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(BM_SortingOperations)->Range(1000, 100000);

// Branch-heavy code - shows branch prediction optimization
static void BM_BranchHeavyCode(benchmark::State& state) {
    const size_t size = static_cast<size_t>(state.range(0));
    std::vector<int> data(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    std::generate(data.begin(), data.end(), [&]() { return dis(gen); });

    for (auto _ : state) {
        long long sum = 0;
        for (size_t i = 0; i < size; ++i) {
            if (data[i] > 50) {
                if (data[i] > 75) {
                    sum += data[i] * 3;
                } else {
                    sum += data[i] * 2;
                }
            } else {
                if (data[i] < 25) {
                    sum -= data[i];
                } else {
                    sum += data[i];
                }
            }
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_BranchHeavyCode)->Range(10000, 1000000);

// String operations - shows string optimization
static void BM_StringOperations(benchmark::State& state) {
    const size_t count = static_cast<size_t>(state.range(0));
    std::vector<std::string> strings;

    for (size_t i = 0; i < count; ++i) {
        strings.push_back("String number " + std::to_string(i) + " with some additional text");
    }

    for (auto _ : state) {
        std::string result;
        for (const auto& str : strings) {
            result += str + " | ";
        }
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_StringOperations)->Range(100, 5000);
