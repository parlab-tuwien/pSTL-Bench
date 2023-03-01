#ifndef MASTER_BENCHMARKS_B1_GROUP_H
#define MASTER_BENCHMARKS_B1_GROUP_H

#include <execution>
#include <ranges>
#include <benchmark/benchmark.h>

#include "../benchmark_prefix.h"
#include "../benchmark_utils.h"

#include "b1_1_for_each_linear.h"
#include "b1_2_for_each_quadratic.h"
#include "b1_3_for_each_quadratic_single_loop.h"
#include "b1_4_for_each_exponential.h"

//region b1_1_for_each_linear

template<class Policy>
static void b1_1_for_each_linear_wrapper(benchmark::State &state) {
    constexpr auto execution_policy = Policy{};

    std::vector<int> x(state.range(0), 1);

    for (auto _: state) {
        B1::b1_1_for_each_linear(execution_policy, x);
    }
}

//endregion b1_1_for_each_linear

//region b1_2_for_each_quadratic

template<class OuterPolicy, class InnerPolicy>
static void b1_2_for_each_quadratic_wrapper(benchmark::State &state) {
    constexpr auto outer_execution_policy = OuterPolicy{};
    constexpr auto inner_execution_policy = InnerPolicy{};

    const auto size = state.range(0);
    const auto input_data = suite::generate_increment<suite::int_vec>(size, 1, 0);

    for (auto _: state) {
        B1::b1_2_for_each_quadratic(outer_execution_policy, inner_execution_policy, input_data);
    }
}

#define B1_2_FOR_EACH_QUADRATIC_WRAPPER(outer) \
    BENCHMARK_TEMPLATE2(b1_2_for_each_quadratic_wrapper,outer,std::execution::sequenced_policy)->Name(BENCHMARK_NAME("b1_2_for_each_quadratic_outer_" xstr(outer) "_seq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 15); \
    BENCHMARK_TEMPLATE2(b1_2_for_each_quadratic_wrapper,outer,std::execution::parallel_policy)->Name(BENCHMARK_NAME("b1_2_for_each_quadratic_outer_" xstr(outer) "_par"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 15); \
    BENCHMARK_TEMPLATE2(b1_2_for_each_quadratic_wrapper,outer,std::execution::parallel_unsequenced_policy)->Name(BENCHMARK_NAME("b1_2_for_each_quadratic_outer_" xstr(outer) "_par_unseq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 15);


//endregion b1_2_for_each_quadratic

//region b1_3_for_each_quadratic_single_loop

template<class Policy>
static void b1_3_for_each_quadratic_single_loop_wrapper(benchmark::State &state) {
    constexpr auto execution_policy = Policy{};

    const auto size = state.range(0);
    const auto input_data = suite::generate_increment<suite::int_vec>(size, 1, 0);

    for (auto _: state) {
        B1::b1_3_for_each_quadratic_single_loop(execution_policy, input_data);
    }
}

//endregion b1_3_for_each_quadratic_single_loop

//region b1_4_for_each_exponential

template<class Policy>
static void b1_4_for_each_exponential_wrapper(benchmark::State &state) {
    constexpr auto execution_policy = Policy{};

    const auto &data = std::ranges::iota_view(1, static_cast<int>(state.range(0)));

    for (auto _: state) {
        B1::b1_4_for_each_exponential(execution_policy, data);
    }
}

//endregion b1_4_for_each_exponential


// Register the function as a benchmark
#define B1_GROUP_BENCHMARKS \
                            \
                            \
    BENCHMARK_TEMPLATE1(b1_1_for_each_linear_wrapper,std::execution::sequenced_policy)->Name(BENCHMARK_NAME("b1_1_for_each_linear_seq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 20); \
    BENCHMARK_TEMPLATE1(b1_1_for_each_linear_wrapper,std::execution::parallel_policy)->Name(BENCHMARK_NAME("b1_1_for_each_linear_par"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 20); \
    BENCHMARK_TEMPLATE1(b1_1_for_each_linear_wrapper,std::execution::parallel_unsequenced_policy)->Name(BENCHMARK_NAME("b1_1_for_each_linear_par_unseq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 20); \
                             \
                             \
    B1_2_FOR_EACH_QUADRATIC_WRAPPER(std::execution::sequenced_policy); \
    B1_2_FOR_EACH_QUADRATIC_WRAPPER(std::execution::parallel_policy); \
    B1_2_FOR_EACH_QUADRATIC_WRAPPER(std::execution::parallel_unsequenced_policy); \
                            \
                            \
    BENCHMARK_TEMPLATE1(b1_3_for_each_quadratic_single_loop_wrapper,std::execution::sequenced_policy)->Name(BENCHMARK_NAME("b1_3_for_each_quadratic_single_loop_seq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 20); \
    BENCHMARK_TEMPLATE1(b1_3_for_each_quadratic_single_loop_wrapper,std::execution::parallel_policy)->Name(BENCHMARK_NAME("b1_3_for_each_quadratic_single_loop_par"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 20); \
    BENCHMARK_TEMPLATE1(b1_3_for_each_quadratic_single_loop_wrapper,std::execution::parallel_unsequenced_policy)->Name(BENCHMARK_NAME("b1_3_for_each_quadratic_single_loop_par_unseq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 5, 1 << 20); \
                            \
                            \
    BENCHMARK_TEMPLATE1(b1_4_for_each_exponential_wrapper,std::execution::sequenced_policy)->Name(BENCHMARK_NAME("b1_4_for_each_exponential_seq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 2, 1 << 5); \
    BENCHMARK_TEMPLATE1(b1_4_for_each_exponential_wrapper,std::execution::parallel_policy)->Name(BENCHMARK_NAME("b1_4_for_each_exponential_par"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 2, 1 << 5); \
    BENCHMARK_TEMPLATE1(b1_4_for_each_exponential_wrapper,std::execution::parallel_unsequenced_policy)->Name(BENCHMARK_NAME("b1_4_for_each_exponential_par_unseq"))->CUSTOM_STATISTICS->RangeMultiplier(2)->Range(1 << 2, 1 << 5);


#endif //MASTER_BENCHMARKS_B1_GROUP_H
