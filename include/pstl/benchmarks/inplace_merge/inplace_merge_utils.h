#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"

namespace benchmark_inplace_merge
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input_data = pstl::generate_increment(execution_policy, size);

		for (auto _ : state)
		{
			std::shuffle(input_data.begin(), input_data.end(), std::mt19937{ std::random_device{}() });

			auto middle = input_data.begin() + (input_data.size() / 2);

			std::sort(input_data.begin(), middle);
			std::sort(middle, input_data.end());

			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input_data.begin(), middle,
			                  input_data.end());

			assert((std::is_sorted(input_data.begin(), input_data.end())));
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input_data));
	}
} // namespace benchmark_inplace_merge


