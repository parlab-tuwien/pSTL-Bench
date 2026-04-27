#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "search_utils.h"

#include "search_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "search_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "search_hpx.h"
#endif

//region search_std
template<class Policy>
static void search_std_wrapper(benchmark::State & state)
{
	benchmark_search::benchmark_wrapper<Policy>(state, benchmark_search::search_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define SEARCH_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(search_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::search"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SEARCH_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define SEARCH_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(search_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::search"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SEARCH_STD_WRAPPER
#endif
//endregion search_std

//region search_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void search_gnu_wrapper(benchmark::State & state)
{
	benchmark_search::benchmark_wrapper<Policy>(state, benchmark_search::search_gnu);
}

#define SEARCH_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(search_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::search"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SEARCH_GNU_WRAPPER
#endif
//endregion search_gnu

//region search_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void search_hpx_wrapper(benchmark::State & state)
{
	benchmark_search::benchmark_wrapper<Policy>(state, benchmark_search::search_hpx);
}

#define SEARCH_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(search_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::search"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define SEARCH_HPX_WRAPPER
#endif
//endregion search_hpx

#define SEARCH_GROUP   \
	SEARCH_SEQ_WRAPPER \
	SEARCH_STD_WRAPPER \
	SEARCH_GNU_WRAPPER \
	SEARCH_HPX_WRAPPER

SEARCH_GROUP