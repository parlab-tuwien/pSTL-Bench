
#pragma once

#include <cmath>

#include <benchmark/benchmark.h>

#include "pstl/utils/utils.h"
#include "pstl/utils/verification.h"

namespace benchmark_for_each
{
#ifndef PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS
#define PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS 1
#endif

	const auto kernel = [](auto & input, const auto Its) {
		volatile size_t I = Its;

		pstl::elem_t a{};

		for (auto i = 0; i < I; ++i)
		{
			a++;
		}

		input = a;
	};

	template<class Policy, class Function>
	static void benchmark_for_each_wrapper(benchmark::State & state, Function && f)
	{
		constexpr auto execution_policy = Policy{};

		const auto & size = state.range(0);

		auto data = pstl::generate_increment(execution_policy, size);

		// Read environment variable PSTL_FOR_EACH_KERNEL_ITS
		std::size_t its = PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS;

		const char * env_p = std::getenv("PSTL_BENCH_FOR_EACH_KERNEL_ITS");
		if (env_p) { its = std::strtoul(env_p, nullptr, 10); }

		std::optional<bool> verification_passed;

		for (auto _ : state)
		{
			pstl::wrap_timing(state, std::forward<Function>(f), execution_policy, data,
			                  [=](auto & elem) { kernel(elem, its); });

			if (not verification_passed.has_value())
			{
				verification_passed = pstl::verify([&]() {
					auto solution = pstl::generate_increment(execution_policy, size);
					std::for_each(std::begin(solution), std::end(solution), [=](auto & elem) { kernel(elem, its); });
					return pstl::are_equivalent(data, solution);
				});
			}
		}

		state.SetBytesProcessed(pstl::computed_bytes(state, data));

		state.counters["kernel_its"] = static_cast<double>(its);
		pstl::set_verification_counter(state, verification_passed);
	}
} // namespace benchmark_for_each
