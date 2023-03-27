
#ifndef MASTER_BENCHMARKS_B6_GROUP_H
#define MASTER_BENCHMARKS_B6_GROUP_H

#include <benchmark/benchmark.h>
#include <vector>
#include <execution>
#include <cassert>

#include "../benchmark_prefix.h"
#include "../benchmark_utils.h"

#include "b6_1_inclusive_scan.h"
#include "b6_2_exclusive_scan.h"


//region b6_1_inclusive_scan

template<class Policy>
static void b6_1_inclusive_scan_wrapper(benchmark::State &state) {
    constexpr auto execution_policy = Policy{};

    const auto &size = state.range(0);

    // vector with values [0,size)
    const auto vec1 = suite::generate_increment<suite::int_vec>(size, 1);

    std::vector<int> res(vec1.size());

    for (auto _: state) {
        B6::b6_1_inclusive_scan(execution_policy, vec1, res);

        state.PauseTiming();
        assert(vec1[0] == res[0]);
        state.ResumeTiming();
    }

    // https://ccfd.github.io/courses/hpc_lab01.html
    const int64_t actual_size_in_bytes = sizeof(int) *
                                         (int64_t(vec1.size()) + int64_t(vec1.size()));

    state.SetBytesProcessed(int64_t(state.iterations()) * actual_size_in_bytes);
}

//endregion b6_1_inclusive_scan


//region b6_2_exclusive_scan

template<class Policy>
static void b6_2_exclusive_scan_wrapper(benchmark::State &state) {
    constexpr auto execution_policy = Policy{};

    const auto &size = state.range(0);

    // vector with values [0,size)
    const auto vec1 = suite::generate_increment<suite::int_vec>(size, 1);

    std::vector<int> res(vec1.size());

    for (auto _: state) {
        B6::b6_2_exclusive_scan(execution_policy, vec1, res);

        state.PauseTiming();
        assert((res[0] == 0));
        assert((res[1] == vec1[0]));
        state.ResumeTiming();
    }

    // https://ccfd.github.io/courses/hpc_lab01.html
    const int64_t actual_size_in_bytes = sizeof(int) *
                                         (int64_t(vec1.size()) + int64_t(vec1.size()));

    state.SetBytesProcessed(int64_t(state.iterations()) * actual_size_in_bytes);
}

//endregion b6_2_exclusive_scan


#define B6_GROUP_BENCHMARKS \
                            \
        BENCHMARK_TEMPLATE1(b6_1_inclusive_scan_wrapper,std::execution::sequenced_policy)->Name(BENCHMARK_NAME("b6_1_inclusive_scan_seq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(MAX_INPUT_SIZE, MAX_INPUT_SIZE); \
        BENCHMARK_TEMPLATE1(b6_1_inclusive_scan_wrapper,std::execution::parallel_policy)->Name(BENCHMARK_NAME("b6_1_inclusive_scan_par"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 2, MAX_INPUT_SIZE);     \
                            \
                            \
        BENCHMARK_TEMPLATE1(b6_2_exclusive_scan_wrapper,std::execution::sequenced_policy)->Name(BENCHMARK_NAME("b6_2_exclusive_scan_seq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(MAX_INPUT_SIZE, MAX_INPUT_SIZE); \
        BENCHMARK_TEMPLATE1(b6_2_exclusive_scan_wrapper,std::execution::parallel_policy)->Name(BENCHMARK_NAME("b6_2_exclusive_scan_par"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 2, MAX_INPUT_SIZE);     \


#endif //MASTER_BENCHMARKS_B6_GROUP_H
