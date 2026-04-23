#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_exclusive_scan
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		const auto input = pstl::generate_increment(execution_policy, size);

		auto output = pstl::get_vector<Policy>(size);

		std::optional<bool> verification_passed;

		for (auto _ : state)
		{
			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input, output);

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() {
					auto solution = pstl::get_vector<Policy>(size);
					std::exclusive_scan(input.begin(), input.end(), solution.begin(), pstl::elem_t{});
					return pstl::are_equivalent(output, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input, output));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_exclusive_scan
