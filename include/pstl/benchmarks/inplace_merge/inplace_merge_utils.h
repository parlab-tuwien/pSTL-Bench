#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_inplace_merge
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input = pstl::generate_increment(execution_policy, size);

		std::optional<bool> verification_passed;

		for (auto _ : state)
		{
			std::shuffle(input.begin(), input.end(), std::mt19937{ std::random_device{}() });

			auto middle = input.begin() + (input.size() / 2);

			std::sort(input.begin(), middle);
			std::sort(middle, input.end());

			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input.begin(), middle, input.end());

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() { return std::is_sorted(input.begin(), input.end()); });
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_inplace_merge
