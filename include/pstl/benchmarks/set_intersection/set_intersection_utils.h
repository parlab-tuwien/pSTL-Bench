#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_set_intersection
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto data1 = pstl::generate_increment(execution_policy, size);
		auto data2 = pstl::generate_increment(execution_policy, size / 2, static_cast<pstl::elem_t>(size / 4),
		                                      static_cast<pstl::elem_t>(1));

		const auto max_size = std::min(data1.size(), data2.size());

		auto output = std::vector<pstl::elem_t>(max_size, std::numeric_limits<pstl::elem_t>::quiet_NaN());

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, data1, data2, output);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					auto solution = std::vector<pstl::elem_t>(max_size, std::numeric_limits<pstl::elem_t>::quiet_NaN());
					std::set_intersection(data1.begin(), data1.end(), data2.begin(), data2.end(), solution.begin());
					return pstl::are_equivalent(output, solution);
				});
			}

			std::fill(output.begin(), output.end(), std::numeric_limits<pstl::elem_t>::quiet_NaN());
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, data1, data2));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_set_intersection
