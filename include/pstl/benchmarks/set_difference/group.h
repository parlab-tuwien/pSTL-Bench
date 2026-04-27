#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "set_difference_utils.h"

#include "set_difference_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "set_difference_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "set_difference_hpx.h"
#include <hpx/executors/parallel_executor.hpp>
#endif

//region set_difference_std
template<class Policy>
static void set_difference_std_wrapper(benchmark::State & state)
{
	benchmark_set_difference::benchmark_wrapper<Policy>(state, benchmark_set_difference::set_difference_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define SET_DIFFERENCE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(set_difference_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::set_difference"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SET_DIFFERENCE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define SET_DIFFERENCE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(set_difference_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::set_difference"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SET_DIFFERENCE_STD_WRAPPER
#endif
//endregion set_difference_std

//region set_difference_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void set_difference_gnu_wrapper(benchmark::State & state)
{
	benchmark_set_difference::benchmark_wrapper<Policy>(state, benchmark_set_difference::set_difference_gnu);
}

#define SET_DIFFERENCE_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(set_difference_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::set_difference"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SET_DIFFERENCE_GNU_WRAPPER
#endif
//endregion set_difference_gnu

//region set_difference_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void set_difference_hpx_wrapper(benchmark::State & state)
{
	benchmark_set_difference::benchmark_wrapper<Policy>(state, benchmark_set_difference::set_difference_hpx);
}

#define SET_DIFFERENCE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(set_difference_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::set_difference"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SET_DIFFERENCE_HPX_WRAPPER
#endif
//endregion set_difference_hpx

#define SET_DIFFERENCE_GROUP   \
	SET_DIFFERENCE_SEQ_WRAPPER \
	SET_DIFFERENCE_STD_WRAPPER \
	SET_DIFFERENCE_GNU_WRAPPER \
	SET_DIFFERENCE_HPX_WRAPPER

SET_DIFFERENCE_GROUP