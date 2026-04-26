#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_copy
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		const auto input = pstl::generate_increment(execution_policy, size);

		auto output = pstl::get_vector<Policy>(size);

		std::optional<bool> verification_result = std::nullopt;

		for (auto _ : state)
		{
			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input, output);

			if (not verification_result.has_value())
			{
				verification_result =
				    pstl::verify([&]() { return std::equal(input.begin(), input.end(), output.begin()); });
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input, output));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_copy
