if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "PGI" AND
        NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVIDIA" AND
        NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVHPC")
    message(WARNING "Make sure you use a NVIDIA compiler. Your compiler ID: ${CMAKE_CXX_COMPILER_ID}")
endif ()

add_compile_definitions(USE_PSTL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdpar=multicore")