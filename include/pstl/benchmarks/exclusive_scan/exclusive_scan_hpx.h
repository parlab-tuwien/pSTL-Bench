#pragma once

#include <hpx/numeric.hpp>

namespace benchmark_exclusive_scan
{
	const auto exclusive_scan_hpx = [](auto && policy, const auto & input, auto & output) {
		hpx::exclusive_scan(policy, input.begin(), input.end(), output.begin(), 0);
	};
} // namespace benchmark_exclusive_scan
