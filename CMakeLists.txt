cmake_minimum_required(VERSION 3.22)
project(pSTL-Bench)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# region options
option(PSTL_BENCH_USE_PAR_ALLOC "Use the parallel allocator to avoid first-touch bottlenecks" OFF)

option(PSTL_BENCH_USE_PAPI "Use PAPI for performance counters" OFF)
option(PSTL_BENCH_USE_LIKWID "Use LIKWID for performance counters" OFF)

set(PSTL_BENCH_BACKEND CACHE STRING "Select the backend to use for std:: algorithms")
set_property(CACHE PSTL_BENCH_BACKEND PROPERTY STRINGS TBB GNU HPX NVHPC_OMP NVHPC_CUDA)
set(PSTL_BENCH_DATA_TYPE CACHE STRING "Select the data type to use for the benchmarks")
set_property(CACHE PSTL_BENCH_DATA_TYPE PROPERTY STRINGS int float double)

if (PSTL_BENCH_DATA_TYPE STREQUAL "")
    set(PSTL_BENCH_DATA_TYPE "double")
    message(STATUS "No data type selected, using default (${PSTL_BENCH_DATA_TYPE})")
endif ()

message(STATUS "Using ${PSTL_BENCH_BACKEND} backend...")
message(STATUS "Using ${PSTL_BENCH_DATA_TYPE} data type...")

# add compile definition if PSTL_ELEM_T is not empty
if (NOT PSTL_BENCH_DATA_TYPE STREQUAL "")
    add_compile_definitions(PSTL_ELEM_T=${PSTL_BENCH_DATA_TYPE})
endif ()
# endregion options

# region Setup CPM (for dependencies)
set(CPM_USE_LOCAL_PACKAGES ON)
set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)

# check if the cache is not defined (also as an environment variable)
if (NOT DEFINED ENV{CPM_SOURCE_CACHE} AND NOT DEFINED CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_BINARY_DIR}/deps_cache/" CACHE PATH "CPM source cache" FORCE)
    message(STATUS "CPM_SOURCE_CACHE not defined, using ${CPM_SOURCE_CACHE}")
else ()
    if (DEFINED ENV{CPM_SOURCE_CACHE})
        message(STATUS "CPM_SOURCE_CACHE (env) defined, using $ENV{CPM_SOURCE_CACHE}")
    else ()
        message(STATUS "CPM_SOURCE_CACHE defined, using ${CPM_SOURCE_CACHE}")
    endif ()
endif ()

include(cmake/get_cpm.cmake)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CPM_SOURCE_CACHE};${CMAKE_MODULE_PATH}")
# endregion Setup CPM (for dependencies)

# region register backends
include(cmake/register_backend.cmake)

register_backend(TBB)
register_backend(HPX)
register_backend(GNU)
register_backend(NVHPC_CUDA)
register_backend(NVHPC_OMP)

# Define the BENCHMARK_COMPILER macro to ${CMAKE_CXX_COMPILER_ID}
add_compile_definitions(PSTL_BENCH_COMPILER="${CMAKE_CXX_COMPILER_ID}")

# load backend specific stuff
if (NOT PSTL_BENCH_BACKEND STREQUAL "")
    use_backend(${PSTL_BENCH_BACKEND})
endif ()
# endregion register backends

# region set default build type to release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif ()
# endregion

file(GLOB SOURCES
        src/*.h
        src/*.cpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        src/main.cpp)

# region benchmark prefix
set(PSTL_BENCH_BENCHMARK_PREFIX ${BACKEND})
add_compile_definitions(PSTL_BENCH_BENCHMARK_PREFIX=${PSTL_BENCH_BENCHMARK_PREFIX})
# endregion benchmark prefix

# region parallel allocator
if (PSTL_BENCH_USE_PAR_ALLOC)
    message(STATUS "Using custom parallel allocator")
    add_compile_definitions(PSTL_BENCH_USE_PAR_ALLOC)
else ()
    message(STATUS "NOT using custom parallel allocator")
endif ()
# endregion parallel allocator

# region input size
if (NOT DEFINED ${PSTL_BENCH_MIN_INPUT_SIZE})
    set(PSTL_BENCH_MIN_INPUT_SIZE 8 CACHE STRING "Minimum input size (#elements) for the benchmarks")
endif ()

if (NOT DEFINED ${PSTL_BENCH_MAX_INPUT_SIZE})
    set(PSTL_BENCH_MAX_INPUT_SIZE 1073741824 CACHE STRING "Maximum input size (#elements) for the benchmarks")
endif ()

add_compile_definitions(PSTL_BENCH_MIN_INPUT_SIZE=${PSTL_BENCH_MIN_INPUT_SIZE})
add_compile_definitions(PSTL_BENCH_MAX_INPUT_SIZE=${PSTL_BENCH_MAX_INPUT_SIZE})
# endregion input size

# region for_each kernel iterations
if (NOT DEFINED ${PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS})
    set(PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS 1 CACHE STRING "Number of iterations for the for_each kernel (can be changed with environment variable PSTL_FOR_EACH_KERNEL_ITS)")
endif ()

add_compile_definitions(PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS=${PSTL_BENCH_DEFAULT_FOR_EACH_KERNEL_ITS})
# endregion for_each kernel iterations

# region GPU enforced transfers
option(PSTL_BENCH_GPU_CONTINUOUS_TRANSFERS "Data will be transferred between host and device before and after each kernel. When OFF, data will be transferred only once." OFF)
if (${PSTL_BENCH_GPU_CONTINUOUS_TRANSFERS})
    add_compile_definitions(PSTL_BENCH_GPU_CONTINUOUS_TRANSFERS)
    message(STATUS "Enforcing continuous transfers between host and device")
else ()
    message(STATUS "Not enforcing continuous transfers between host and device")
endif ()
# endregion GPU enforced transfers

# region google benchmark stuff
CPMAddPackage(
        NAME benchmark
        GITHUB_REPOSITORY google/benchmark
        VERSION 1.8.3
        OPTIONS
        "BENCHMARK_ENABLE_TESTING Off" # disable tests
        "BENCHMARK_ENABLE_LIBPFM ON" # enable performance counters
        "BENCHMARK_ENABLE_WERROR OFF" # make sure google benchmark builds no mather what
)

if (benchmark_ADDED)
    # enable the C++ standard to avoid errors
    set_target_properties(benchmark PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
    list(APPEND LINK_LIBRARIES benchmark::benchmark)
elseif (TARGET benchmark::benchmark)
    list(APPEND LINK_LIBRARIES benchmark::benchmark)
else ()
    message(FATAL_ERROR "Failed to add google benchmark")
endif ()
# endregion google benchmark stuff

# region PAPI
if (PSTL_BENCH_USE_PAPI)
    add_compile_definitions(PSTL_BENCH_USE_PAPI)
    find_package(PAPI REQUIRED)
    if (PAPI_FOUND)
        message(STATUS "PAPI found")
        message(STATUS "PAPI include dirs: ${PAPI_INCLUDE_DIRS}")
        message(STATUS "PAPI libraries: ${PAPI_LIBRARIES}")
    else ()
        message(FATAL_ERROR "PAPI not found")
    endif ()
    list(APPEND LINK_LIBRARIES ${PAPI_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${PAPI_INCLUDE_DIRS})
endif ()
# endregion PAPI

# region LIKWID
if (PSTL_BENCH_USE_LIKWID)
    add_compile_definitions(PSTL_BENCH_USE_LIKWID)
    find_package(likwid REQUIRED)
    if (LIKWID_FOUND)
        message(STATUS "LIKWID found")
        message(STATUS "LIKWID include dirs: ${LIKWID_INCLUDE_DIRS}")
        message(STATUS "LIKWID libraries: ${LIKWID_LIBRARIES}")
        # Add -DLIKWID_PERFMON to the compile options
        target_compile_definitions(${PROJECT_NAME} PRIVATE LIKWID_PERFMON)
    else ()
        message(FATAL_ERROR "LIKWID not found")
    endif ()
    list(APPEND LINK_LIBRARIES ${LIKWID_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIKWID_INCLUDE_DIRS})
endif ()
# endregion LIKWID

# Add numa library
list(APPEND LINK_LIBRARIES numa)

# link libraries
foreach (LIB ${LINK_LIBRARIES})
    message(STATUS "Linking ${LIB}...")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB})
endforeach ()

target_include_directories(${PROJECT_NAME} PRIVATE include/)

# append backend specific compile options
target_compile_options(${PROJECT_NAME} PRIVATE ${BACKEND_COMPILE_OPTIONS})

# append further optimization options
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native)
endif ()
