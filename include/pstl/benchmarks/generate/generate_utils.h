#pragma once

#include <algorithm>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_generate
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input = pstl::generate_increment(execution_policy, size);

		static const auto generator = []() {
			return pstl::elem_t{};
		};

		std::optional<bool> verification_passed;

		for (auto _ : state)
		{
			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input, generator);

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() {
					auto solution = pstl::generate_increment(execution_policy, size);
					std::generate(std::begin(solution), std::end(solution), generator);
					return pstl::are_equivalent(input, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_generate
