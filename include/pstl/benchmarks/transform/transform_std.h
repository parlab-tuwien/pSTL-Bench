#pragma once

#include <algorithm>
#include <execution>

namespace benchmark_transform
{
	const auto transform_std = [](auto && policy, auto & input, auto & output, auto && f) {
		std::transform(policy, input.begin(), input.end(), output.begin(), f);
	};
} // namespace benchmark_transform
