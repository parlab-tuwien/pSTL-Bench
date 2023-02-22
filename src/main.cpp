#include <benchmark/benchmark.h>
#include "benchmarks/b1/b1_group.h"
#include "benchmarks/b2/b2_group.h"
#include "benchmarks/b3/b3_group.h"
#include "benchmarks/b4/b4_group.h"
#include "benchmarks/b5/b5_group.h"
#include "benchmarks/b6/b6_group.h"

B1_GROUP_BENCHMARKS
B2_GROUP_BENCHMARKS
B3_GROUP_BENCHMARKS
B4_GROUP_BENCHMARKS
B5_GROUP_BENCHMARKS
B6_GROUP_BENCHMARKS

// Run the benchmark
BENCHMARK_MAIN();