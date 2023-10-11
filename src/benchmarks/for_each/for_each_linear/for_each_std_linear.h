#ifndef PSTL_BENCH_FOR_EACH_STD_LINEAR_H
#define PSTL_BENCH_FOR_EACH_STD_LINEAR_H


#include <algorithm>
#include <benchmark/benchmark.h>
#include <benchmark_utils.h>
#include <cmath>
#include <vector>

namespace benchmark_for_each
{

	const auto for_each_std_linear = [](auto && policy, const auto & input_data, auto && f) {
		std::for_each(policy, input_data.begin(), input_data.end(), f);
	};

}

#endif //PSTL_BENCH_FOR_EACH_STD_LINEAR_H
