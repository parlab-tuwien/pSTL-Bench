#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_all_of
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input = pstl::generate_increment(execution_policy, size);

		const auto condition = [](const auto & val) {
			return val > 0;
		};

		std::optional<bool> verification_result = std::nullopt;

		for (auto _ : state)
		{
			const auto output = pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input, condition);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					const auto solution = std::all_of(input.begin(), input.end(), condition);
					return pstl::are_equivalent(output, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_all_of
