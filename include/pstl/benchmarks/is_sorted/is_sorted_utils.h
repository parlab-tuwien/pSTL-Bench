#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_is_sorted
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
			const auto output = pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input);

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() {
					const auto solution = std::is_sorted(input.begin(), input.end());
					return pstl::are_equivalent(output, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_is_sorted
