#pragma once

#include <hpx/algorithm.hpp>

namespace benchmark_transform
{

	const auto transform_hpx = [](auto && policy, auto & input, auto & output, auto && f) {
		hpx::transform(policy, input.begin(), input.end(), output.begin(), f);
	};

} // namespace benchmark_transform
