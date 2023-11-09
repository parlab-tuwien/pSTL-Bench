#ifndef PSTL_BENCH_EQUAL_UTILS_H
#define PSTL_BENCH_EQUAL_UTILS_H

#include <numeric>

#include <benchmark/benchmark.h>

#include <benchmark_utils.h>

namespace benchmark_equal
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		const auto input_data = suite::generate_increment(execution_policy, size, 1);

		auto output = input_data;

		for (auto _ : state)
		{
			WRAP_TIMING(F(execution_policy, input_data, output);)
		}

		// https://ccfd.github.io/courses/hpc_lab01.html
		const int64_t actual_size_in_bytes = sizeof(int) * (2 * int64_t(input_data.size()));

		state.SetBytesProcessed(int64_t(state.iterations()) * actual_size_in_bytes);
	}
} // namespace benchmark_equal

#endif //PSTL_BENCH_EQUAL_UTILS_H
