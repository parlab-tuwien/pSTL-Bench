#ifndef PSTL_BENCH_MISMATCH_UTILS_H
#define PSTL_BENCH_MISMATCH_UTILS_H

#include <numeric>

#include <benchmark/benchmark.h>

#include <benchmark_utils.h>

namespace benchmark_mismatch
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto data1 = suite::generate_increment(execution_policy, size, 1);

		static auto rd  = std::random_device{};
		static auto rng = std::mt19937{ rd() };
		auto        gen = std::uniform_int_distribution<int64_t>{ 0, size - 1 };

		for (auto _ : state)
		{
			auto data2 = data1;

			const auto idx = gen(rng);
			data2[idx]     = data1[idx] + 1;

			WRAP_TIMING(const auto output = F(execution_policy, data1, data2));

			const auto solution = std::mismatch(data1.begin(), data1.end(), data2.begin());

			assert((solution.first - data1.begin()) == (output.first - data1.begin()));
		}

		// https://ccfd.github.io/courses/hpc_lab01.html
		const int64_t actual_size_in_bytes = sizeof(int) * (2 * int64_t(data1.size()));

		state.SetBytesProcessed(int64_t(state.iterations()) * actual_size_in_bytes);
	}
} // namespace benchmark_mismatch

#endif //PSTL_BENCH_MISMATCH_UTILS_H
