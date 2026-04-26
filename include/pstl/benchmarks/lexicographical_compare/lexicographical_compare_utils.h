#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_lexicographical_compare
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		const auto data1 = pstl::generate_increment(execution_policy, size);
		const auto data2 = pstl::generate_increment(execution_policy, size);

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			const auto output = pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, data1, data2);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					const auto result =
					    std::lexicographical_compare(data1.begin(), data1.end(), data2.begin(), data2.end());
					return pstl::are_equivalent(result, output);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, data1, data2));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_lexicographical_compare
