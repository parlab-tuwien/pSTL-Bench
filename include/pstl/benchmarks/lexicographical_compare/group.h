#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "lexicographical_compare_utils.h"

#include "lexicographical_compare_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "lexicographical_compare_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "lexicographical_compare_hpx.h"
#endif

//region lexicographical_compare_std
template<class Policy>
static void lexicographical_compare_std_wrapper(benchmark::State & state)
{
	benchmark_lexicographical_compare::benchmark_wrapper<Policy>(
	    state, benchmark_lexicographical_compare::lexicographical_compare_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define LEXICOGRAPHICAL_COMPARE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(lexicographical_compare_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::lexicographical_compare"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define LEXICOGRAPHICAL_COMPARE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define LEXICOGRAPHICAL_COMPARE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(lexicographical_compare_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::lexicographical_compare"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define LEXICOGRAPHICAL_COMPARE_STD_WRAPPER
#endif
//endregion lexicographical_compare_std

//region lexicographical_compare_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void lexicographical_compare_gnu_wrapper(benchmark::State & state)
{
	benchmark_lexicographical_compare::benchmark_wrapper<Policy>(
	    state, benchmark_lexicographical_compare::lexicographical_compare_gnu);
}

#define LEXICOGRAPHICAL_COMPARE_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(lexicographical_compare_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::lexicographical_compare"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define LEXICOGRAPHICAL_COMPARE_GNU_WRAPPER
#endif
//endregion lexicographical_compare_gnu

//region lexicographical_compare_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void lexicographical_compare_hpx_wrapper(benchmark::State & state)
{
	benchmark_lexicographical_compare::benchmark_wrapper<Policy>(
	    state, benchmark_lexicographical_compare::lexicographical_compare_hpx);
}

#define LEXICOGRAPHICAL_COMPARE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(lexicographical_compare_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::lexicographical_compare"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define LEXICOGRAPHICAL_COMPARE_HPX_WRAPPER
#endif
//endregion lexicographical_compare_hpx

#define LEXICOGRAPHICAL_COMPARE_GROUP   \
	LEXICOGRAPHICAL_COMPARE_SEQ_WRAPPER \
	LEXICOGRAPHICAL_COMPARE_STD_WRAPPER \
	LEXICOGRAPHICAL_COMPARE_GNU_WRAPPER \
	LEXICOGRAPHICAL_COMPARE_HPX_WRAPPER

LEXICOGRAPHICAL_COMPARE_GROUP