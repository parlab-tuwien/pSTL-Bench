#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "none_of_utils.h"

#include "none_of_std.h"

#ifdef PSTL_BENCH_USE_HPX
#include "none_of_hpx.h"
#endif

//region none_of_std
template<class Policy>
static void none_of_std_wrapper(benchmark::State & state)
{
	benchmark_none_of::benchmark_wrapper<Policy>(state, benchmark_none_of::none_of_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define NONE_OF_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(none_of_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::none_of"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define NONE_OF_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define NONE_OF_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(none_of_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::none_of"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define NONE_OF_STD_WRAPPER
#endif
//endregion none_of_std

//region none_of_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void none_of_hpx_wrapper(benchmark::State & state)
{
	benchmark_none_of::benchmark_wrapper<Policy>(state, benchmark_none_of::none_of_hpx);
}

#define NONE_OF_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(none_of_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::none_of"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define NONE_OF_HPX_WRAPPER
#endif
//endregion none_of_hpx

#define NONE_OF_GROUP   \
	NONE_OF_SEQ_WRAPPER \
	NONE_OF_STD_WRAPPER \
	NONE_OF_HPX_WRAPPER

NONE_OF_GROUP