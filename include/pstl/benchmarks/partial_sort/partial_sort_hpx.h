#pragma once

#include <hpx/algorithm.hpp>

namespace benchmark_partial_sort
{
	const auto partial_sort_hpx = [](auto && policy, auto begin, auto middle, auto end) {
		return hpx::partial_sort(policy, begin, middle, end);
	};
} // namespace benchmark_partial_sort
