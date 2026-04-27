add_compile_definitions(PSTL_BENCH_USE_PSTL)

add_compile_definitions(PSTL_BENCH_BACKEND="rocThrust")

set(STDPAR_BASE_FLAGS "--hipstdpar")

# If no "--offload-arch=" flag is provided
if(NOT CMAKE_CXX_FLAGS MATCHES "--offload-arch=")
    # Warn the user that the default architecture will be used
    message(WARNING "No --offload-arch= flag provided. Defaulting to gfx90a. Please specify the target architecture for better performance. Provide the flag as follows: -DCMAKE_CXX_FLAGS=\"--offload-arch=<arch>\"")
    list(APPEND BACKEND_COMPILE_OPTIONS ${STDPAR_BASE_FLAGS} "--offload-arch=gfx90a" ${CMAKE_CXX_FLAGS})
else()
    list(APPEND BACKEND_COMPILE_OPTIONS ${STDPAR_BASE_FLAGS} ${CMAKE_CXX_FLAGS})
endif()
