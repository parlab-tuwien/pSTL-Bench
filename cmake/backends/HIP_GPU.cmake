add_compile_definitions(PSTL_BENCH_USE_PSTL)

add_compile_definitions(PSTL_BENCH_BACKEND="rocThrust")

SET(CMAKE_CXX_FLAGS "--hipstdpar ${CMAKE_CXX_FLAGS}")
