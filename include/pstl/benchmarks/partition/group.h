#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "partition_utils.h"

#include "partition_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "partition_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "partition_hpx.h"
#endif

//region partition_std
template<class Policy>
static void partition_std_wrapper(benchmark::State & state)
{
	benchmark_partition::benchmark_wrapper<Policy>(state, benchmark_partition::partition_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define PARTITION_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(partition_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::partition"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define PARTITION_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define PARTITION_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(partition_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::partition"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define PARTITION_STD_WRAPPER
#endif
//endregion partition_std

//region partition_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void partition_gnu_wrapper(benchmark::State & state)
{
	benchmark_partition::benchmark_wrapper<Policy>(state, benchmark_partition::partition_gnu);
}

#define PARTITION_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(partition_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::partition"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define PARTITION_GNU_WRAPPER
#endif
//endregion partition_gnu

//region partition_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void partition_hpx_wrapper(benchmark::State & state)
{
	benchmark_partition::benchmark_wrapper<Policy>(state, benchmark_partition::partition_hpx);
}

#define PARTITION_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(partition_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::partition"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define PARTITION_HPX_WRAPPER
#endif
//endregion partition_hpx

#define PARTITION_GROUP   \
	PARTITION_SEQ_WRAPPER \
	PARTITION_STD_WRAPPER \
	PARTITION_GNU_WRAPPER \
	PARTITION_HPX_WRAPPER

PARTITION_GROUP