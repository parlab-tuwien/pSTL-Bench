#pragma once

#include <algorithm>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_fill
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
			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input, pstl::elem_t{});

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() {
					return std::all_of(input.begin(), input.end(),
					                   [](const auto & val) { return pstl::are_equivalent(val, pstl::elem_t{}); });
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_fill
