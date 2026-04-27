#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "exclusive_scan_utils.h"

#include "exclusive_scan_std.h"

#ifdef PSTL_BENCH_USE_HPX
#include "exclusive_scan_hpx.h"
#endif

//region exclusive_scan_std
template<class Policy>
static void exclusive_scan_std_wrapper(benchmark::State & state)
{
	benchmark_exclusive_scan::benchmark_wrapper<Policy>(state, benchmark_exclusive_scan::exclusive_scan_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define EXCLUSIVE_SCAN_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(exclusive_scan_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::exclusive_scan"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EXCLUSIVE_SCAN_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define EXCLUSIVE_SCAN_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(exclusive_scan_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::exclusive_scan"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EXCLUSIVE_SCAN_STD_WRAPPER
#endif
//endregion exclusive_scan_std

//region exclusive_scan_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void exclusive_scan_hpx_wrapper(benchmark::State & state)
{
	benchmark_exclusive_scan::benchmark_wrapper<Policy>(state, benchmark_exclusive_scan::exclusive_scan_hpx);
}

#define EXCLUSIVE_SCAN_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(exclusive_scan_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::exclusive_scan"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EXCLUSIVE_SCAN_HPX_WRAPPER
#endif
//endregion exclusive_scan_hpx

#define EXCLUSIVE_SCAN_GROUP   \
	EXCLUSIVE_SCAN_SEQ_WRAPPER \
	EXCLUSIVE_SCAN_STD_WRAPPER \
	EXCLUSIVE_SCAN_HPX_WRAPPER

EXCLUSIVE_SCAN_GROUP