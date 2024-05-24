#pragma once

#include <hpx/algorithm.hpp>

namespace benchmark_search
{
	const auto search_hpx = [](auto && policy, const auto & first1, const auto & last1, const auto & first2,
	                           const auto & last2) {
		return hpx::search(policy, first1, last1, first2, last2);
	};
} // namespace benchmark_search


