#pragma once

#include <algorithm>

namespace benchmark_sort
{
	const auto sort_std = [](auto && policy, auto & input) {
#warning \
    "Using std::sort(parallel_policy) may explode in memory usage. See: https://github.com/llvm/llvm-project/issues/78956#issue-2093630075"
		std::sort(policy, input.begin(), input.end());
	};
} // namespace benchmark_sort
