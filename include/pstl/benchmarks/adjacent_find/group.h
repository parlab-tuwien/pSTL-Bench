#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "adjacent_find_utils.h"

#include "adjacent_find_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "adjacent_find_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "adjacent_find_hpx.h"
#endif

//region adjacent_find_std
template<class Policy>
static void adjacent_find_std_wrapper(benchmark::State & state)
{
	benchmark_adjacent_find::benchmark_wrapper<Policy>(state, benchmark_adjacent_find::adjacent_find_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define ADJACENT_FIND_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(adjacent_find_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::adjacent_find"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_FIND_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define ADJACENT_FIND_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(adjacent_find_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::adjacent_find"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_FIND_STD_WRAPPER
#endif
//endregion adjacent_find_std

//region adjacent_find_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void adjacent_find_gnu_wrapper(benchmark::State & state)
{
	benchmark_adjacent_find::benchmark_wrapper<Policy>(state, benchmark_adjacent_find::adjacent_find_gnu);
}

#define ADJACENT_FIND_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(adjacent_find_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::adjacent_find"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_FIND_GNU_WRAPPER
#endif
//endregion adjacent_find_gnu

//region adjacent_find_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void adjacent_find_hpx_wrapper(benchmark::State & state)
{
	benchmark_adjacent_find::benchmark_wrapper<Policy>(state, benchmark_adjacent_find::adjacent_find_hpx);
}

#define ADJACENT_FIND_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(adjacent_find_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::adjacent_find"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_FIND_HPX_WRAPPER
#endif
//endregion adjacent_find_hpx

#define ADJACENT_FIND_GROUP   \
	ADJACENT_FIND_SEQ_WRAPPER \
	ADJACENT_FIND_STD_WRAPPER \
	ADJACENT_FIND_GNU_WRAPPER \
	ADJACENT_FIND_HPX_WRAPPER

ADJACENT_FIND_GROUP