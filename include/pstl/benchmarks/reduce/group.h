#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "reduce_utils.h"

#include "reduce_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "reduce_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "reduce_hpx.h"
#endif

//region reduce_std
template<class Policy>
static void reduce_std_wrapper(benchmark::State & state)
{
	benchmark_reduce::benchmark_wrapper<Policy>(state, benchmark_reduce::reduce_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define REDUCE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(reduce_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::reduce"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define REDUCE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define REDUCE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(reduce_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::reduce"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define REDUCE_STD_WRAPPER
#endif
//endregion reduce_std

//region reduce_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void reduce_gnu_wrapper(benchmark::State & state)
{
	benchmark_reduce::benchmark_wrapper<Policy>(state, benchmark_reduce::reduce_gnu);
}

#define REDUCE_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(reduce_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::reduce"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define REDUCE_GNU_WRAPPER
#endif
//endregion reduce_gnu

//region reduce_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void reduce_hpx_wrapper(benchmark::State & state)
{
	benchmark_reduce::benchmark_wrapper<Policy>(state, benchmark_reduce::reduce_hpx);
}

#define REDUCE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(reduce_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::reduce"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define REDUCE_HPX_WRAPPER
#endif
//endregion reduce_hpx

#define REDUCE_GROUP   \
	REDUCE_SEQ_WRAPPER \
	REDUCE_STD_WRAPPER \
	REDUCE_GNU_WRAPPER \
	REDUCE_HPX_WRAPPER

REDUCE_GROUP