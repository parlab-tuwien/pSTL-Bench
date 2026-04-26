#pragma once

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_transform_exclusive_scan
{
	const auto kernel = [](const auto & input) {
		return input * input;
	};

	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && f)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input  = pstl::generate_increment(execution_policy, size);
		auto output = input;

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			std::ignore = pstl::wrap_timing(state, std::forward<Function>(f), execution_policy, input, output, kernel);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					auto solution = input;
					std::ignore   = std::transform_exclusive_scan(input.begin(), input.end(), solution.begin(),
					                                              pstl::elem_t{}, std::plus<>(), kernel);
					return pstl::are_equivalent(output, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_transform_exclusive_scan
