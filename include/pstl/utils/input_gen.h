#pragma once

#include <algorithm>
#include <random>
#include <vector>

#include "pstl/utils/elem_t.h"

#ifdef PSTL_BENCH_USE_PAR_ALLOC
#include "pstl/utils/par_alloc.h"
#include <execution>

#ifdef PSTL_BENCH_USE_HPX
#include <hpx/algorithm.hpp>
#endif

#ifdef PSTL_BENCH_USE_GNU
#include <parallel/algorithm>
#endif

#endif

namespace pstl
{
#ifdef PSTL_BENCH_USE_PAR_ALLOC
	template<typename VALUE_TYPE, typename ExecutionPolicy,
	         typename = std::enable_if_t<std::is_execution_policy<ExecutionPolicy>::value>>
	using vector = std::vector<VALUE_TYPE, pstl::par_alloc<VALUE_TYPE, ExecutionPolicy>>;
#else
	template<typename VALUE_TYPE, typename ExecutionPolicy>
	using vector = std::vector<VALUE_TYPE>;
#endif

	/**
     * Allows to create a vector using the allocation strategy configured
     * @tparam ExecutionPolicy the execution policy to use. Only considered when using the flag `PSTL_BENCH_USE_PAR_ALLOC`
     * @tparam Container the container type to create
     * @tparam ValueType the value type of the container
     * @tparam Size_type the size type of the container
     * @param size the actual size of the container we want to create
     * @return  the newly created container
     */
	template<typename ExecutionPolicy, typename ValueType = pstl::elem_t,
	         typename Container = pstl::vector<ValueType, ExecutionPolicy>,
	         typename Size_type = typename Container::size_type>
	Container get_vector(const Size_type size)
	{
#ifdef PSTL_BENCH_USE_PAR_ALLOC
		constexpr auto execution_policy = ExecutionPolicy{};

		const pstl::par_alloc<ValueType, ExecutionPolicy> allocator(execution_policy);
		Container                                         v(size, allocator);
#else
		Container v(size);
#endif
		return v;
	}

	/**
     * Fills the given container with the provided value
     * @tparam ExecutionPolicy the execution policy to use for filling
     * @tparam Container  the container type to fill
     * @tparam ValueType  the value type of the given container
     * @param container the container to fill
     * @param value the value that should be filled in
     */
	template<typename ExecutionPolicy, typename ValueType = pstl::elem_t,
	         typename Container = pstl::vector<ValueType, ExecutionPolicy>>
	void fill_init(Container & container, const ValueType value)
	{
		constexpr auto execution_policy = ExecutionPolicy{};
		std::fill(execution_policy, container.begin(), container.end(), value);
	}

	template<typename ExecutionPolicy, typename T>
	auto generate_uniform_dist(const std::size_t & size, const T & lower_bound, const T & upper_bound)
	{
#if ONLY_GPU
		constexpr auto execution_policy = std::execution::seq;
#else
		constexpr auto execution_policy = ExecutionPolicy{};
#endif

		std::mt19937                     mt_engine(std::random_device{}());
		std::uniform_real_distribution<> dist(lower_bound, upper_bound);

		auto v = pstl::get_vector<ExecutionPolicy, pstl::vector<T, ExecutionPolicy>>(size);

		std::generate(execution_policy, v.begin(), v.end(), [&]() { return (T) dist(mt_engine); });

		return v;
	}

	/**
     * Generates a Container of type Container that contains size elements where the first element is start
     * and then every next element is incremented by increment
     *
     * the vector contains elements [start,start+(size*increment) )
     *
     * @tparam ValueType the value type of the container
     * @tparam Container the container type
     * @param start the number to start the range from
     * @param size the number of elements
     * @param increment the increment to use
     */
	template<typename ExecutionPolicy, typename ValueType = pstl::elem_t,
	         typename Container = pstl::vector<ValueType, ExecutionPolicy>,
	         typename Size_type = typename Container::size_type>
	Container generate_increment(const ExecutionPolicy & execution_policy, const Size_type & size,
	                             const ValueType start, const ValueType increment)
	{
		auto v = pstl::get_vector<ExecutionPolicy, ValueType, Container>(size);

		auto begin_ptr = v.data();

		auto body = [=](auto & val) {
			const auto i = &val - begin_ptr;
			val          = start + i * increment;
		};

#ifdef PSTL_BENCH_USE_PAR_ALLOC
#if defined(PSTL_BENCH_USE_HPX)
		hpx::for_each(execution_policy, v.begin(), v.end(), body);
#elif defined(PSTL_BENCH_USE_GNU)
		__gnu_parallel::for_each(v.begin(), v.end(), body);
#else
		std::for_each(execution_policy, v.begin(), v.end(), body);
#endif
#else
		std::for_each(v.begin(), v.end(), body);
#endif

		return v;
	}

	/**
     * Generates a container of type Container that contains size elements where the first element is start_val
     * and then every next element is decremented by decrement
     *
     * @tparam ExecutionPolicy the executionpolicy to use for the creation
     * @tparam ValueType the value type of the container
     * @tparam Container the container type
     * @param size the number of elements
     * @param start_val the start val
     * @param decrement the value to use to decrement
     */
	template<typename ExecutionPolicy, typename ValueType = pstl::elem_t,
	         typename Container = pstl::vector<ValueType, ExecutionPolicy>,
	         typename Size_type = typename Container::size_type>
	Container generate_decrement(const ExecutionPolicy & execution_policy, const Size_type & size,
	                             const ValueType start_val, const ValueType decrement = 1)
	{
		return generate_increment(execution_policy, size, start_val, -decrement);
	}

	/**
     * Generates a vector of type T that contains size elements where the first element is 0
     * and then every next element is incremented by increment
     *
     * @tparam ValueType the type of the vector
     * @param size the number of elements
     * @param increment the increment to use
     */
	template<typename ExecutionPolicy, typename ValueType = pstl::elem_t,
	         typename Container = pstl::vector<ValueType, ExecutionPolicy>,
	         typename Size_type = typename Container::size_type>
	Container generate_increment(const ExecutionPolicy & execution_policy, const Size_type & size,
	                             const ValueType increment)
	{
		return pstl::generate_increment(execution_policy, size, {}, increment);
	}

	/**
     * Generates a vector of type T that contains size elements where the first element is 0
     * and then every next element is incremented by increment
     *
     * @tparam ValueType the type of the vector
     * @param size the number of elements
     */
	template<typename ExecutionPolicy, typename ValueType = pstl::elem_t,
	         typename Container = pstl::vector<ValueType, ExecutionPolicy>,
	         typename Size_type = typename Container::size_type>
	Container generate_increment(const ExecutionPolicy & execution_policy, const Size_type & size)
	{
		return pstl::generate_increment(execution_policy, size, ValueType{ 1 }, ValueType{ 1 });
	}
} // namespace pstl