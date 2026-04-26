#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_min_element
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input = pstl::generate_increment(execution_policy, size);
		std::shuffle(input.begin(), input.end(), std::mt19937{ std::random_device{}() });

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			const auto output = pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					const auto solution = std::min_element(input.begin(), input.end());
					return pstl::are_equivalent(*solution, *output);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_min_element
