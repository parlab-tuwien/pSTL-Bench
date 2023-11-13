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

		const auto data1 = suite::generate_increment(execution_policy, size, 1);

		auto data2 = data1;

		for (auto _ : state)
		{
			WRAP_TIMING(F(execution_policy, data1, data2);)
		}

		state.SetBytesProcessed(suite::computed_bytes(state, data1, data2));
	}
} // namespace benchmark_equal

#endif //PSTL_BENCH_EQUAL_UTILS_H
