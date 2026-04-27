#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "min_element_utils.h"

#include "min_element_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "min_element_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "min_element_hpx.h"
#endif

//region min_element_std
template<class Policy>
static void min_element_std_wrapper(benchmark::State & state)
{
	benchmark_min_element::benchmark_wrapper<Policy>(state, benchmark_min_element::min_element_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define MIN_ELEMENT_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(min_element_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::min_element"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define MIN_ELEMENT_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define MIN_ELEMENT_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(min_element_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::min_element"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define MIN_ELEMENT_STD_WRAPPER
#endif
//endregion min_element_std

//region min_element_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void min_element_gnu_wrapper(benchmark::State & state)
{
	benchmark_min_element::benchmark_wrapper<Policy>(state, benchmark_min_element::min_element_gnu);
}

#define MIN_ELEMENT_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(min_element_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::min_element"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define MIN_ELEMENT_GNU_WRAPPER
#endif
//endregion min_element_gnu

//region min_element_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void min_element_hpx_wrapper(benchmark::State & state)
{
	benchmark_min_element::benchmark_wrapper<Policy>(state, benchmark_min_element::min_element_hpx);
}

#define MIN_ELEMENT_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(min_element_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::min_element"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define MIN_ELEMENT_HPX_WRAPPER
#endif
//endregion min_element_hpx

#define MIN_ELEMENT_GROUP   \
	MIN_ELEMENT_SEQ_WRAPPER \
	MIN_ELEMENT_STD_WRAPPER \
	MIN_ELEMENT_GNU_WRAPPER \
	MIN_ELEMENT_HPX_WRAPPER

MIN_ELEMENT_GROUP