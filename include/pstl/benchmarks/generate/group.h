#pragma once

#include "pstl/utils/benchmark_naming.h"

#include "generate_utils.h"

#include "generate_std.h"

#ifdef PSTL_BENCH_USE_GNU
#include "generate_gnu.h"
#endif

#ifdef PSTL_BENCH_USE_HPX
#include "generate_hpx.h"
#endif

//region generate_std
template<class Policy>
static void generate_std_wrapper(benchmark::State & state)
{
	benchmark_generate::benchmark_wrapper<Policy>(state, benchmark_generate::generate_std);
}

#ifdef PSTL_BENCH_USE_SEQ
#define GENERATE_SEQ_WRAPPER                                                    \
	BENCHMARK_TEMPLATE1(generate_std_wrapper, std::execution::sequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME_WITH_BACKEND("SEQ", "std::generate"))  \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define GENERATE_SEQ_WRAPPER
#endif

#ifdef PSTL_BENCH_USE_PSTL
#define GENERATE_STD_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(generate_std_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("std::generate"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define GENERATE_STD_WRAPPER
#endif
//endregion generate_std

//region generate_gnu
#ifdef PSTL_BENCH_USE_GNU
template<class Policy>
static void generate_gnu_wrapper(benchmark::State & state)
{
	benchmark_generate::benchmark_wrapper<Policy>(state, benchmark_generate::generate_gnu);
}

#define GENERATE_GNU_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(generate_gnu_wrapper, std::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("gnu::generate"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define GENERATE_GNU_WRAPPER
#endif
//endregion generate_gnu

//region generate_hpx
#ifdef PSTL_BENCH_USE_HPX
template<class Policy>
static void generate_hpx_wrapper(benchmark::State & state)
{
	benchmark_generate::benchmark_wrapper<Policy>(state, benchmark_generate::generate_hpx);
}

#define GENERATE_HPX_WRAPPER                                                               \
	BENCHMARK_TEMPLATE1(generate_hpx_wrapper, hpx::execution::parallel_unsequenced_policy) \
	    ->Name(PSTL_BENCH_BENCHMARK_NAME("hpx::generate"))                                 \
	    ->PSTL_BENCH_BENCHMARK_PARAMETERS
#else
#define GENERATE_HPX_WRAPPER
#endif
//endregion generate_hpx

#define GENERATE_GROUP   \
	GENERATE_SEQ_WRAPPER \
	GENERATE_STD_WRAPPER \
	GENERATE_GNU_WRAPPER \
	GENERATE_HPX_WRAPPER

GENERATE_GROUP