#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "adjacent_difference_utils.h"

#include "adjacent_difference_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "adjacent_difference_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "adjacent_difference_hpx.h"
#endif

//region adjacent_difference_std
template<class Policy>
static void adjacent_difference_std_wrapper(benchmark::State & state)
{
	benchmark_adjacent_difference::benchmark_wrapper<Policy>(state,
	                                                         benchmark_adjacent_difference::adjacent_difference_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define ADJACENT_DIFFERENCE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(adjacent_difference_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::adjacent_difference"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_DIFFERENCE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define ADJACENT_DIFFERENCE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(adjacent_difference_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::adjacent_difference"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_DIFFERENCE_STD_WRAPPER
#endif
//endregion adjacent_difference_std

//region adjacent_difference_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void adjacent_difference_gnu_wrapper(benchmark::State & state)
{
	benchmark_adjacent_difference::benchmark_wrapper<Policy>(state,
	                                                         benchmark_adjacent_difference::adjacent_difference_gnu);
}

#define ADJACENT_DIFFERENCE_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(adjacent_difference_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::adjacent_difference"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_DIFFERENCE_GNU_WRAPPER
#endif
//endregion adjacent_difference_gnu

//region adjacent_difference_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void adjacent_difference_hpx_wrapper(benchmark::State & state)
{
	benchmark_adjacent_difference::benchmark_wrapper<Policy>(state,
	                                                         benchmark_adjacent_difference::adjacent_difference_hpx);
}

#define ADJACENT_DIFFERENCE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(adjacent_difference_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::adjacent_difference"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define ADJACENT_DIFFERENCE_HPX_WRAPPER
#endif

#define ADJACENT_DIFFERENCE_GROUP   \
	ADJACENT_DIFFERENCE_SEQ_WRAPPER \
	ADJACENT_DIFFERENCE_STD_WRAPPER \
	ADJACENT_DIFFERENCE_GNU_WRAPPER \
	ADJACENT_DIFFERENCE_HPX_WRAPPER


ADJACENT_DIFFERENCE_GROUP