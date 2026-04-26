#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_merge
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input  = pstl::generate_increment(execution_policy, size);
		auto output = input;

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			std::shuffle(input.begin(), input.end(), std::mt19937{ std::random_device{}() });

			auto middle = input.begin() + (input.size() / 2);

			std::sort(input.begin(), middle);
			std::sort(middle, input.end());

			pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, input.begin(), middle, middle,
			                  input.end(), output.begin());

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					return std::is_sorted(output.begin(), output.end()) and
					       std::includes(output.begin(), output.end(), input.begin(), middle) and
					       std::includes(output.begin(), output.end(), middle, input.end());
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_merge
