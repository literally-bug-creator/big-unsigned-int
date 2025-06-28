.PHONY: all debug release test bench clean

BUILD_DIR = build
CXX = clang++
JOBS = $(shell nproc)

all: release

debug: clean
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) ..
	@cd $(BUILD_DIR) && cmake --build . --parallel $(JOBS)

release: clean
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) ..
	@cd $(BUILD_DIR) && cmake --build . --parallel $(JOBS)

test:
	@./$(BUILD_DIR)/tests/run_tests

bench:
	@./$(BUILD_DIR)/benchmarks/run_benchmark --benchmark_time_unit=ms

clean:
	@rm -rf $(BUILD_DIR)
