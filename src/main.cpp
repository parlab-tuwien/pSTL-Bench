#ifdef USE_HPX
#include <hpx/hpx_main.hpp>
#endif

#include "benchmarks/adjacent_difference/group.h"
#include "benchmarks/adjacent_find/group.h"
#include "benchmarks/count_if/group.h"
#include "benchmarks/exclusive_scan/group.h"
#include "benchmarks/find/group.h"
#include "benchmarks/for_each/group.h"
#include "benchmarks/inclusive_scan/group.h"
#include "benchmarks/reduce/group.h"
#include "benchmarks/sort/group.h"

#include <thread>

#include <benchmark/benchmark.h>

#ifdef USE_TBB
#include <tbb_thread_control.h>
#endif

ADJACENT_DIFFERENCE_GROUP
ADJACENT_FIND_GROUP
COUNT_IF_GROUP
FIND_GROUP
REDUCE_GROUP
FOR_EACH_GROUP
INCLUSIVE_SCAN_GROUP
EXCLUSIVE_SCAN_GROUP
SORT_GROUP

// Run the benchmark
int main(int argc, char ** argv)
{
#ifdef USE_TBB
	auto tbbThreadControl = init_tbb_thread_control();
#endif

	char   arg0_default[] = "benchmark";
	char * args_default   = arg0_default;
	if (!argv)
	{
		argc = 1;
		argv = &args_default;
	}

	benchmark::AddCustomContext("std::thread::hardware_concurrency()",
	                            std::to_string(std::thread::hardware_concurrency()));

#ifdef USE_TBB
	benchmark::AddCustomContext("tbb #threads", std::to_string(tbb::global_control::active_value(
	                                                tbb::global_control::max_allowed_parallelism)));
#endif


#if defined(USE_OMP) or defined(USE_GNU_PSTL)
	benchmark::AddCustomContext("omp_get_max_threads()", std::to_string(omp_get_max_threads()));
#endif

	benchmark::Initialize(&argc, argv);
	if (benchmark::ReportUnrecognizedArguments(argc, argv)) { return 1; }
	benchmark::RunSpecifiedBenchmarks();
	benchmark::Shutdown();

#ifdef USE_TBB
	tbbThreadControl = nullptr;
#endif

	return 0;
}
