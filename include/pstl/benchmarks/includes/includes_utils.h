#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_includes
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		const auto input = pstl::generate_increment(execution_policy, size);

		const auto subset = pstl::generate_increment(execution_policy, size / 2, static_cast<pstl::elem_t>(size / 4),
		                                             static_cast<pstl::elem_t>(1));

		std::optional<bool> verification_passed;

		for (auto _ : state)
		{
			const auto output = pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input, subset);

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() {
					const auto solution = std::includes(input.begin(), input.end(), subset.begin(), subset.end());
					return pstl::are_equivalent(output, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input, subset));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_includes
