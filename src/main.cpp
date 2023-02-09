#include <benchmark/benchmark.h>
#include "benchmarks/b1/b1_for_each_linear.h"

static void b1_for_each_linear_seq(benchmark::State &state) {
    // Perform setup here
    for (auto _: state) {
        // This code gets timed
        state.PauseTiming();

        std::vector<int> x(state.range(0), 1);

        state.ResumeTiming();
        b1_for_each_linear(std::execution::seq, x);
    }
}

static void b1_for_each_linear_par(benchmark::State &state) {
    // Perform setup here
    for (auto _: state) {
        // This code gets timed
        state.PauseTiming();

        std::vector<int> x(state.range(0), 1);

        state.ResumeTiming();
        b1_for_each_linear(std::execution::par, x);
    }
}

static void b1_for_each_linear_par_unseq(benchmark::State &state) {
    // Perform setup here
    for (auto _: state) {
        // This code gets timed
        state.PauseTiming();

        std::vector<int> x(state.range(0), 1);

        state.ResumeTiming();
        b1_for_each_linear(std::execution::par_unseq, x);
    }
}



// Register the function as a benchmark
BENCHMARK(b1_for_each_linear_seq)->RangeMultiplier(2)->Range(1 << 5, 1 << 20);
BENCHMARK(b1_for_each_linear_par)->RangeMultiplier(2)->Range(1 << 5, 1 << 20);
BENCHMARK(b1_for_each_linear_par_unseq)->RangeMultiplier(2)->Range(1 << 5, 1 << 20);

// Run the benchmark
BENCHMARK_MAIN();