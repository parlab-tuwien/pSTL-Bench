#ifndef PSTL_BENCH_FOR_EACH_STD_EXPONENTIAL_H
#define PSTL_BENCH_FOR_EACH_STD_EXPONENTIAL_H

#include <algorithm>
#include <benchmark/benchmark.h>
#include <benchmark_utils.h>
#include <cmath>
#include <iostream>
#include <ranges>
#include <vector>

namespace benchmark_for_each
{

	template<class ExecutionPolicy>
	struct FIB
	{
		typedef suite::base_type<ExecutionPolicy> BASE_POLICY;

		ExecutionPolicy m_policy;

		explicit FIB(ExecutionPolicy & policy) : m_policy(policy) {}

		void operator()(const int & index)
		{
			if (index <= 0) { return; }

			double value = std::tan(index) + std::cos(index);
			benchmark::DoNotOptimize(value);


			auto next = suite::get_vec<BASE_POLICY>(2);
			next      = { index - 1, index - 2 };

			std::for_each(m_policy, next.begin(), next.end(), FIB{ m_policy });
		}
	};

	const auto for_each_std_exponential = [](auto && policy, const auto & input_data) {
		std::for_each(policy, input_data.begin(), input_data.end(), FIB{ policy });
	};

} // namespace benchmark_for_each

#endif //PSTL_BENCH_FOR_EACH_STD_EXPONENTIAL_H
