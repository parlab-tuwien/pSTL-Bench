#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "inplace_merge_utils.h"

#include "inplace_merge_std.h"

#ifdef PSTL_BENCH_USE_HPX
#include "inplace_merge_hpx.h"
#endif

//region inplace_merge_std
template<class Policy>
static void inplace_merge_std_wrapper(benchmark::State & state)
{
	benchmark_inplace_merge::benchmark_wrapper<Policy>(state, benchmark_inplace_merge::inplace_merge_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define INPLACE_MERGE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(inplace_merge_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::inplace_merge"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define INPLACE_MERGE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define INPLACE_MERGE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(inplace_merge_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::inplace_merge"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define INPLACE_MERGE_STD_WRAPPER
#endif
//endregion inplace_merge_std

//region inplace_merge_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void inplace_merge_hpx_wrapper(benchmark::State & state)
{
	benchmark_inplace_merge::benchmark_wrapper<Policy>(state, benchmark_inplace_merge::inplace_merge_hpx);
}

#define INPLACE_MERGE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(inplace_merge_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::inplace_merge"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define INPLACE_MERGE_HPX_WRAPPER
#endif
//endregion inplace_merge_hpx

#define INPLACE_MERGE_GROUP   \
	INPLACE_MERGE_SEQ_WRAPPER \
	INPLACE_MERGE_STD_WRAPPER \
	INPLACE_MERGE_HPX_WRAPPER

INPLACE_MERGE_GROUP