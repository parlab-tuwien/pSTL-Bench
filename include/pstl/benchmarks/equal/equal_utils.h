#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_equal
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		const auto data1 = pstl::generate_increment(execution_policy, size);

		auto data2 = data1;

		std::optional<bool> verification_passed;

		for (auto _ : state)
		{
			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, data1, data2);

			if (not verification_passed.has_value())
			{
				verification_passed =
				    pstl::verify([&]() { return std::equal(data1.begin(), data1.end(), data2.begin()); });
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, data1, data2));

		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_equal
