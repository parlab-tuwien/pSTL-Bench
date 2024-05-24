#pragma once

#include <execution>
#include <numeric>

#include <benchmark/benchmark.h>

namespace benchmark_search
{
	const auto search_std = [](auto && policy, const auto & first1, const auto & last1, const auto & first2,
	                           const auto & last2) {
		return std::search(policy, first1, last1, first2, last2);
	};
} // namespace benchmark_search


