#pragma once

#include <numeric>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_mismatch
{
	template<class Policy, class Function>
	static void benchmark_wrapper(benchmark::State & state, Function && F)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto data1 = pstl::generate_increment(execution_policy, size);

		static auto rd  = std::random_device{};
		static auto rng = std::mt19937{ rd() };
		auto        gen = std::uniform_int_distribution<int64_t>{ 0, size - 1 };

		auto data2 = data1;

		std::optional<bool> verification_result;

		for (auto _ : state)
		{
			const auto idx = gen(rng);
			// Introduce a mismatch at a random position
			data2[idx]     = data1[idx] + 1;

			const auto output = pstl::wrap_timing(state, std::forward<Function>(F), execution_policy, data1, data2);

			if (not verification_result.has_value())
			{
				verification_result = pstl::verify([&]() {
					const auto solution = std::mismatch(data1.begin(), data1.end(), data2.begin());
					return (solution.first - data1.begin()) == (output.first - data1.begin());
				});
			}

			// Undo the mismatch for the next iteration
			data2[idx] = data1[idx];
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, data1, data2));

		pstl::set_verification_counter(state, verification_result);
	}
} // namespace benchmark_mismatch
