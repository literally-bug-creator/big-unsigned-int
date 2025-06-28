.PHONY: all debug release test benchmark format-check format static-analysis clean

BUILD_DIR = build
CXX = clang++

all: release

debug:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) ..
	@cd $(BUILD_DIR) && make -j$(shell nproc)

release:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) ..
	@cd $(BUILD_DIR) && make -j$(shell nproc)

test:
	@cd $(BUILD_DIR) && ./tests/run_tests

benchmark:
	@cd $(BUILD_DIR) && ./benchmarks/run_benchmark --benchmark_time_unit=ms

format-check:
	@find . -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.cc" | \
	grep -E "(big_unsigned_int|tests|benchmarks)" | \
	grep -v "third-party" | \
	xargs clang-format --dry-run --Werror --style=file

format:
	@find . -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.cc" | \
	grep -E "(big_unsigned_int|tests|benchmarks)" | \
	grep -v "third-party" | \
	xargs clang-format -i --style=file

static-analysis: debug
	@find big_unsigned_int -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | \
	xargs clang-tidy -p $(BUILD_DIR) --quiet

clean:
	@rm -rf $(BUILD_DIR)
