#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "copy_if_utils.h"

#include "copy_if_std.h"

#ifdef PSTL_BENCH_USE_HPX
#include "copy_if_hpx.h"
#endif

//region copy_if_std
template<class Policy>
static void copy_if_std_wrapper(benchmark::State & state)
{
	benchmark_copy_if::benchmark_wrapper<Policy>(state, benchmark_copy_if::copy_if_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define COPY_IF_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(copy_if_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::copy_if"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COPY_IF_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define COPY_IF_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(copy_if_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::copy_if"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COPY_IF_STD_WRAPPER
#endif
//endregion copy_if_std

//region copy_if_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void copy_if_hpx_wrapper(benchmark::State & state)
{
	benchmark_copy_if::benchmark_wrapper<Policy>(state, benchmark_copy_if::copy_if_hpx);
}

#define COPY_IF_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(copy_if_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::copy_if"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define COPY_IF_HPX_WRAPPER
#endif
//endregion copy_if_hpx

#define COPY_IF_GROUP COPY_IF_SEQ_WRAPPER COPY_IF_STD_WRAPPER COPY_IF_HPX_WRAPPER

COPY_IF_GROUP