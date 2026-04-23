#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <type_traits>

#include <benchmark/benchmark.h>

#ifdef PSTL_BENCH_USE_PAR_ALLOC
#include "pstl/utils/par_alloc.h"
#endif

#include "pstl/utils/input_gen.h"
#include "pstl/utils/timing.h"

#define PSTL_BENCH_CUSTOM_STATISTICS                                                                                   \
	ComputeStatistics("max", [](const auto & v) -> double { return *(std::max_element(std::begin(v), std::end(v))); }) \
	    -> ComputeStatistics("min", [](const auto & v) -> double {                                                     \
		    return *(std::min_element(std::begin(v), std::end(v)));                                                    \
	    }) -> UseManualTime()

#define PSTL_BENCH_BENCHMARK_PARAMETERS                               \
	PSTL_BENCH_CUSTOM_STATISTICS->RangeMultiplier(2)                  \
	    ->Range(PSTL_BENCH_MIN_INPUT_SIZE, PSTL_BENCH_MAX_INPUT_SIZE) \
	    ->UseManualTime();

namespace pstl
{
	/**
	 * Computes (an estimation) the amount of bytes used by a container
	 * @param container the container to compute the size for
	 * @return the amount of (estimated) bytes used by the container
	 */
	static auto container_size(const auto & container)
	{
		using base_type = std::decay_t<decltype(container)>;
		return sizeof(base_type) * container.size() + sizeof(container);
	}

	/**
	 * Compute the amount of bytes computed by a benchmark
	 *
	 * @param state the benchmark state
	 * @param containers the containers used by the benchmark
	 * @return the amount of bytes computed by the benchmark
	 */
	static auto computed_bytes(const ::benchmark::State & state, const auto &... containers)
	{
		std::size_t bytes = 0;

		const auto accumulate_bytes = [&bytes](const auto & container) {
			bytes += pstl::container_size(container);
		};

		// Use fold expression to iterate over all containers even when they have different types
		(accumulate_bytes(containers), ...);

		return bytes * state.iterations();
	}
} // namespace pstl
