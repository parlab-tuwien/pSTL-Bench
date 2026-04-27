#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "count_utils.h"

#include "count_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "count_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "count_hpx.h"
#endif

//region count_std
template<class Policy>
static void count_std_wrapper(benchmark::State & state)
{
	benchmark_count::benchmark_wrapper<Policy>(state, benchmark_count::count_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define COUNT_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(count_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::count"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COUNT_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define COUNT_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(count_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::count"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COUNT_STD_WRAPPER
#endif
//endregion count_std

//region count_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void count_gnu_wrapper(benchmark::State & state)
{
	benchmark_count::benchmark_wrapper<Policy>(state, benchmark_count::count_gnu);
}

#define COUNT_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(count_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::count"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COUNT_GNU_WRAPPER
#endif
//endregion count_gnu

#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void count_hpx_wrapper(benchmark::State & state)
{
	benchmark_count::benchmark_wrapper<Policy>(state, benchmark_count::count_hpx);
}

#define COUNT_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(count_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::count"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COUNT_HPX_WRAPPER
#endif
//endregion count_hpx

#define COUNT_GROUP   \
	COUNT_SEQ_WRAPPER \
	COUNT_STD_WRAPPER \
	COUNT_GNU_WRAPPER \
	COUNT_HPX_WRAPPER

COUNT_GROUP