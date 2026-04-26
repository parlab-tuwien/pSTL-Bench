#pragma once

#include <execution>
#include <numeric>

#include <benchmark/benchmark.h>

namespace benchmark_partial_sort
{
	const auto partial_sort_std = [](auto && policy, auto begin, auto middle, auto end) {
		return std::partial_sort(policy, begin, middle, end);
	};
} // namespace benchmark_partial_sort
