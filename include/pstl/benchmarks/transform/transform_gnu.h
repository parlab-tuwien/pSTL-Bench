#pragma once

#include <parallel/algorithm>

namespace benchmark_transform
{

	const auto transform_gnu = []([[maybe_unused]] auto && policy, auto & input, auto & output, auto && f) {
		__gnu_parallel::transform(input.begin(), input.end(), output.begin(), f);
	};

} // namespace benchmark_transform
