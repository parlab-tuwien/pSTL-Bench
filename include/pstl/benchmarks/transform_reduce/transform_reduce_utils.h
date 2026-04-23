#pragma once

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_transform_reduce
{
	const auto transform_kernel = [](const auto & input) {
		return input * input;
	};

	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && f)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto input = pstl::generate_increment(execution_policy, size);

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			const auto output =
			    pstl::wrap_timing(state, std::forward<Function>(f), execution_policy, input, transform_kernel);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					const auto solution = std::transform_reduce(input.cbegin(), input.cend(), pstl::elem_t{},
					                                            std::plus<>(), transform_kernel);
					return pstl::are_equivalent(solution, output);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, input));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_transform_reduce
