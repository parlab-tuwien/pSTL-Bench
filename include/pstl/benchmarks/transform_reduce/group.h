#pragma once

#include "pstl/utils/benchmark_naming.h"

#include <benchmark/benchmark.h>

#include "transform_reduce_utils.h"

#include "transform_reduce_std.h"

#ifdef PSTL_BENCH_USE_HPX
#include "transform_reduce_hpx.h"
#endif

//region transform_reduce
template<class Policy>
static void transform_reduce_std_wrapper(benchmark::State & state)
{
	benchmark_transform_reduce::benchmark_wrapper<Policy>(state, benchmark_transform_reduce::transform_reduce_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define TRANSFORM_REDUCE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(transform_reduce_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::transform_reduce"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_REDUCE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define TRANSFORM_REDUCE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(transform_reduce_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::transform_reduce"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_REDUCE_STD_WRAPPER
#endif
//endregion transform_reduce

//region transform_reduce_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void transform_reduce_hpx_wrapper(benchmark::State & state)
{
	benchmark_transform_reduce::benchmark_wrapper<Policy>(state, benchmark_transform_reduce::transform_reduce_hpx);
}

#define TRANSFORM_REDUCE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(transform_reduce_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::transform_reduce"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define TRANSFORM_REDUCE_HPX_WRAPPER
#endif
//endregion transform_reduce_hpx

#define TRANSFORM_REDUCE_GROUP   \
	TRANSFORM_REDUCE_SEQ_WRAPPER \
	TRANSFORM_REDUCE_STD_WRAPPER \
	TRANSFORM_REDUCE_HPX_WRAPPER

TRANSFORM_REDUCE_GROUP