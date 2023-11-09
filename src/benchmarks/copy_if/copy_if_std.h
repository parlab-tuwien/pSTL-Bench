#ifndef PSTL_BENCH_COPY_IF_STD_H
#define PSTL_BENCH_COPY_IF_STD_H

#include <execution>
#include <numeric>

#include <benchmark/benchmark.h>

namespace benchmark_copy_if
{
	const auto copy_if_std = [](auto && policy, const auto & container, auto & output, auto && condition) {
		std::copy_if(policy, container.begin(), container.end(), output.begin(), condition);
	};
} // namespace benchmark_copy_if

#endif //PSTL_BENCH_COPY_IF_STD_H
