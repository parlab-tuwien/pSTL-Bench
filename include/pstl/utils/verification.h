#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include <cstdlib>

namespace pstl
{
	static constexpr const char * ENV_VERIFY     = "PSTL_BENCH_VERIFY";
	static const bool             VERIFY_ENABLED = []() {
        const char * env = std::getenv(ENV_VERIFY);
        if (env)
        {
            std::string val(env);
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            return val == "1" || val == "true";
        }
        return false;
	}();

	static constexpr const char * ENV_VERIFY_VERBOSE = "PSTL_BENCH_VERIFY_VERBOSE";
	static const bool             VERIFY_VERBOSE     = []() {
        const char * env = std::getenv(ENV_VERIFY_VERBOSE);
        if (env)
        {
            std::string val(env);
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            return val == "1" || val == "true";
        }
        return false;
	}();

	static constexpr const char * VERIFICATION_COUNTER_NAME = "verification_passed";
	void                          set_verification_counter(::benchmark::State & state, std::optional<bool> passed)
	{
		if (VERIFY_ENABLED) { state.counters[VERIFICATION_COUNTER_NAME] = passed.value_or(false) ? 1 : 0; }
	}

	namespace detail
	{
		[[noreturn]] inline void assert_fail(const char * expr, const char * file, int line)
		{
			std::cerr << "Assertion failed: " << expr << " at " << file << ':' << line << '\n';
			std::abort();
		}
	} // namespace detail
} // namespace pstl

#define PSTL_BENCH_ASSERT(expr)                                                  \
	do                                                                           \
	{                                                                            \
		if (!(expr)) { ::pstl::detail::assert_fail(#expr, __FILE__, __LINE__); } \
	} while (false)

/**
 * Conditionally verify benchmark results based on the environment variable "PSTL_BENCH_VERIFY".
 *
 * If the environment variable is set to "1", "true", or "TRUE" (case-insensitive),
 * the verification expression is executed. Otherwise, verification is skipped.
 *
 * Usage:
 *     PSTL_BENCH_VERIFY(expr);
 *
 * @param expr Expression to execute for verification (e.g., an assert or check).
 */
#ifndef PSTL_BENCH_VERIFY
#define PSTL_BENCH_VERIFY(expr)                                                                                   \
	do                                                                                                            \
	{                                                                                                             \
		if (not ::pstl::VERIFY_ENABLED)                                                                           \
		{                                                                                                         \
			std::cout << "Skipping benchmark verification (set PSTL_BENCH_VERIFY=1 to enable)" << __FILE__ << ":" \
			          << __LINE__ << '\n';                                                                        \
			break;                                                                                                \
		}                                                                                                         \
		std::cout << "Verifying benchmark results..." << __FILE__ << ":" << __LINE__ << '\n';                     \
		PSTL_BENCH_ASSERT(expr);                                                                                  \
	} while (false)
#endif

namespace pstl
{
	/**
     * Verify a condition and return the result as an optional boolean.
     * @param expr Expression to evaluate.
     * @return Optional boolean indicating verification result.
     */
	template<typename Func>
	inline auto verify(Func && expr) -> std::optional<bool>
	{
		if (not VERIFY_ENABLED)
		{
			return std::nullopt; // Verification is disabled
		}

		return std::optional<bool>(expr());
	}

	template<typename T, typename = void>
	struct is_streamable : std::false_type
	{};

	template<typename T>
	struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream &>() << std::declval<T>())>> : std::true_type
	{};

	template<typename T>
	struct print_wrapper
	{
		const T & v;

		friend std::ostream & operator<<(std::ostream & os, const print_wrapper & wrapper)
		{
			if constexpr (is_streamable<T>::value) { return os << wrapper.v; }
			else
			{
				return os << "<unprintable>";
			}
		}
	};

	// Helper function to enable deduction
	template<typename T>
	auto wrap(const T & v)
	{
		return print_wrapper<T>{ v };
	}


	static void report_mismatch(const auto & v1, const auto & v2)
	{
		if (VERIFY_VERBOSE)
		{
			std::cerr << "Values are not equivalent: " << wrap(v1) << " != " << wrap(v2) << std::endl;
		}
	}

	/**
	 * Check if two values are equivalent, i.e. if they are equal or if they are within a certain tolerance
	 * @param v1 First value
	 * @param v2 Second value
	 * @return
	 */
	template<typename T, typename U, std::enable_if_t<std::is_integral_v<T> and std::is_integral_v<U>, bool> = true>
	static bool are_equivalent(const T & v1, const U & v2)
	{
		const auto are_eq = v1 == v2;

		if (not are_eq and VERIFY_VERBOSE) { report_mismatch(v1, v2); }

		return are_eq;
	}

	/**
	 * Check if two values are equivalent, i.e. if they are equal or if they are within a certain tolerance
	 * @param v1 First value
	 * @param v2 Second value
	 * @return
	 */
	template<typename T, typename U,
	         std::enable_if_t<std::is_floating_point_v<T> and std::is_floating_point_v<U>, bool> = true>
	static bool are_equivalent(const T & v1, const U & v2)
	{
		using fp_t = std::decay_t<decltype(v1)>;

		static const auto ABSOLUTE_TOLERANCE = []() {
			const char * env = std::getenv("PSTL_BENCH_ABS_TOL");
			return env ? std::strtod(env, nullptr) : static_cast<double>(std::numeric_limits<fp_t>::epsilon());
		}();
		static const auto RELATIVE_TOLERANCE = []() {
			const char * env = std::getenv("PSTL_BENCH_REL_TOL");
			return env ? std::strtod(env, nullptr) : 0.01; // Default relative tolerance of 1%
		}();

		const auto abs_error = std::abs(v1 - v2);
		const auto rel_error = abs_error / std::max(std::abs(v1), std::abs(v2));

		const auto are_eq = abs_error <= ABSOLUTE_TOLERANCE or rel_error <= RELATIVE_TOLERANCE;

		if (not are_eq and VERIFY_VERBOSE)
		{
			std::cerr << "Values are not equivalent: " << wrap(v1) << " != " << wrap(v2)
			          << ". Abs. error: " << abs_error << ", rel. error: " << rel_error
			          << ". Abs. tol.: " << ABSOLUTE_TOLERANCE << ", rel. tol.: " << RELATIVE_TOLERANCE << std::endl;
		}

		return are_eq;
	}

	template<typename T, typename = void>
	struct has_begin : std::false_type
	{};

	template<typename T>
	struct has_begin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type
	{};

	template<typename T>
	inline constexpr bool has_begin_v = has_begin<T>::value;

	/**
	 * Check if two containers are equivalent element-wise using are_equivalent.
	 * Returns true if all corresponding elements are equivalent and containers have the same size.
	 * @param c1 First container
	 * @param c2 Second container
	 * @return true if all elements are equivalent, false otherwise
	 */
	template<typename Container1, typename Container2,
	         std::enable_if_t<pstl::has_begin_v<Container1> && pstl::has_begin_v<Container2>, bool> = true>
	static bool are_equivalent(const Container1 & c1, const Container2 & c2)
	{
		if (std::size(c1) != std::size(c2))
		{
			if (VERIFY_VERBOSE)
			{
				std::cerr << "Containers have different sizes: " << std::size(c1) << " != " << std::size(c2)
				          << std::endl;
			}

			return false;
		}

		return std::equal(std::begin(c1), std::end(c1), std::begin(c2),
		                  [](const auto & elem1, const auto & elem2) { return are_equivalent(elem1, elem2); });
	}

	/**
	 * Check if two values are equivalent, i.e. if they are equal or if they are within a certain tolerance
	 * @param v1 First value
	 * @param v2 Second value
	 * @return
	 */
	template<typename T, typename U,
	         std::enable_if_t<not std::is_arithmetic_v<T> and not pstl::has_begin_v<T> and
	                              not std::is_arithmetic_v<U> and not pstl::has_begin_v<U>,
	                          bool> = true>
	static bool are_equivalent(const T & v1, const U & v2)
	{
		const auto are_eq = v1 == v2;

		if (not are_eq and VERIFY_VERBOSE) { report_mismatch(v1, v2); }

		return are_eq;
	}
} // namespace pstl
