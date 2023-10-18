#include "benchmarks/count_if/group.h"
#include "benchmarks/find/group.h"
#include "benchmarks/for_each/group.h"
#include "benchmarks/reduce/group.h"
#include "benchmarks/sort/group.h"

// #include "benchmarks/index_based_iterations/b9_group.h"
#include "benchmarks/optimization_scans/b6_group.h"
#include "benchmarks/sequential_fallback/b4_group.h"
#include "benchmarks/specialized_techniques/b5_group.h"
#include "benchmarks/specific_versus_custom/b7_group.h"

#include <thread>

#include <benchmark/benchmark.h>

#ifdef USE_TBB
// we have the case that we use gcc. this means tbb will be used for parallel stl
// potential thread limits have to be configured

#include <tbb_thread_control.h>

#endif

#ifdef COUNT_IF_GROUP
COUNT_IF_GROUP
#endif
#ifdef FIND_GROUP
FIND_GROUP
#endif
#ifdef REDUCE_GROUP
REDUCE_GROUP
#endif
#ifdef FOR_EACH_GROUP
FOR_EACH_GROUP
#endif
#ifdef SORT_GROUP
SORT_GROUP
#endif

#ifdef SEQUENTIAL_FALLBACK_GROUP
SEQUENTIAL_FALLBACK_GROUP
#endif
#ifdef SPECIALIZED_TECHNIQUES_GROUP
SPECIALIZED_TECHNIQUES_GROUP
#endif
#ifdef OPTIMIZED_SCAN_GROUP
OPTIMIZED_SCAN_GROUP
#endif
#ifdef SPECIFIC_VS_CUSTOM_GROUP
SPECIFIC_VS_CUSTOM_GROUP
#endif
#ifdef INDEX_BASED_ITERATIONS_GROUP
INDEX_BASED_ITERATIONS_GROUP
#endif

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
