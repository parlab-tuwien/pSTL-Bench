#ifndef PSTL_BENCH_REDUCE_STD_H
#define PSTL_BENCH_REDUCE_STD_H

#include <algorithm>
#include <execution>

#include <benchmark/benchmark.h>

namespace benchmark_reduce
{
	const auto reduce_std = [](auto && policy, const auto & container) {
		return std::reduce(policy, container.begin(), container.end(), pstl::elem_t{});
	};
} // namespace benchmark_reduce

#endif //PSTL_BENCH_REDUCE_STD_H
