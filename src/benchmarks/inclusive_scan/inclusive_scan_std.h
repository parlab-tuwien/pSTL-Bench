#ifndef PSTL_BENCH_INCLUSIVE_SCAN_STD_H
#define PSTL_BENCH_INCLUSIVE_SCAN_STD_H

#include <execution>
#include <numeric>

#include <benchmark/benchmark.h>

namespace benchmark_inclusive_scan
{
	const auto inclusive_scan_std = [](auto && policy, const auto & container, auto & output) {
		std::inclusive_scan(policy, container.begin(), container.end(), output.begin());
	};
} // namespace benchmark_inclusive_scan

#endif //PSTL_BENCH_INCLUSIVE_SCAN_STD_H
