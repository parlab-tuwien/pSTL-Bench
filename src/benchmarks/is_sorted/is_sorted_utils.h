#ifndef PSTL_BENCH_IS_SORTED_UTILS_H
#define PSTL_BENCH_IS_SORTED_UTILS_H

#include <numeric>

#include <benchmark/benchmark.h>

#include <benchmark_utils.h>

namespace benchmark_is_sorted
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input_data = suite::generate_increment(execution_policy, size, 1);

		for (auto _ : state)
		{
			WRAP_TIMING(F(execution_policy, input_data);)
		}

		state.SetBytesProcessed(suite::computed_bytes(state, input_data));
	}
} // namespace benchmark_is_sorted

#endif //PSTL_BENCH_IS_SORTED_UTILS_H
