#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "equal_utils.h"

#include "equal_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "equal_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "equal_hpx.h"
#endif

//region equal_std
template<class Policy>
static void equal_std_wrapper(benchmark::State & state)
{
	benchmark_equal::benchmark_wrapper<Policy>(state, benchmark_equal::equal_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define EQUAL_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(equal_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::equal"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EQUAL_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define EQUAL_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(equal_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::equal"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EQUAL_STD_WRAPPER
#endif
//endregion equal_std

//region equal_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void equal_gnu_wrapper(benchmark::State & state)
{
	benchmark_equal::benchmark_wrapper<Policy>(state, benchmark_equal::equal_gnu);
}

#define EQUAL_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(equal_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::equal"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EQUAL_GNU_WRAPPER
#endif
//endregion equal_gnu

//region equal_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void equal_hpx_wrapper(benchmark::State & state)
{
	benchmark_equal::benchmark_wrapper<Policy>(state, benchmark_equal::equal_hpx);
}

#define EQUAL_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(equal_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::equal"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define EQUAL_HPX_WRAPPER
#endif
//endregion equal_hpx

#define EQUAL_GROUP   \
	EQUAL_SEQ_WRAPPER \
	EQUAL_STD_WRAPPER \
	EQUAL_GNU_WRAPPER \
	EQUAL_HPX_WRAPPER

EQUAL_GROUP