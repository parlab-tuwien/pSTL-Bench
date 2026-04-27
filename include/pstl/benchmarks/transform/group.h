#pragma once

#include "pstl/utils/benchmark_naming.h"

#include <benchmark/benchmark.h>

#include "transform_utils.h"

#include "transform_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "transform_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "transform_hpx.h"
#endif

//region transform
template<class Policy>
static void transform_std_wrapper(benchmark::State & state)
{
	benchmark_transform::benchmark_wrapper<Policy>(state, benchmark_transform::transform_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define TRANSFORM_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(transform_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::transform"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define TRANSFORM_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(transform_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::transform"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_STD_WRAPPER
#endif
//endregion transform

//region transform_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void transform_gnu_wrapper(benchmark::State & state)
{
	benchmark_transform::benchmark_wrapper<Policy>(state, benchmark_transform::transform_gnu);
}

#define TRANSFORM_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(transform_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::transform"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_GNU_WRAPPER
#endif
//endregion transform_gnu

//region transform_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void transform_hpx_wrapper(benchmark::State & state)
{
	benchmark_transform::benchmark_wrapper<Policy>(state, benchmark_transform::transform_hpx);
}

#define TRANSFORM_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(transform_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::transform"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_HPX_WRAPPER
#endif
//endregion transform_hpx

#define TRANSFORM_GROUP   \
	TRANSFORM_SEQ_WRAPPER \
	TRANSFORM_STD_WRAPPER \
	TRANSFORM_GNU_WRAPPER \
	TRANSFORM_HPX_WRAPPER

TRANSFORM_GROUP