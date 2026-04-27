#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "includes_utils.h"

#include "includes_std.h"

#ifdef PSTL_BENCH_USE_HPX
#include "includes_hpx.h"
#endif

//region includes_std
template<class Policy>
static void includes_std_wrapper(benchmark::State & state)
{
	benchmark_includes::benchmark_wrapper<Policy>(state, benchmark_includes::includes_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define INCLUDES_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(includes_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::includes"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define INCLUDES_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define INCLUDES_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(includes_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::includes"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define INCLUDES_STD_WRAPPER
#endif
//endregion includes_std

//region includes_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void includes_hpx_wrapper(benchmark::State & state)
{
	benchmark_includes::benchmark_wrapper<Policy>(state, benchmark_includes::includes_hpx);
}

#define INCLUDES_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(includes_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::includes"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define INCLUDES_HPX_WRAPPER
#endif
//endregion includes_hpx

#define INCLUDES_GROUP   \
	INCLUDES_SEQ_WRAPPER \
	INCLUDES_STD_WRAPPER \
	INCLUDES_HPX_WRAPPER

INCLUDES_GROUP