cmake_minimum_required(VERSION 3.15)
project(zstd_workload LANGUAGES CXX)

set(ZSTD_ROOT "${CMAKE_SOURCE_DIR}/../zstd")

add_executable(workload
    workload.cpp
    compress_workload.cpp
    decompress_workload.cpp
)

target_include_directories(workload PRIVATE
    ${ZSTD_ROOT}/lib
)

target_link_libraries(workload PRIVATE
    ${ZSTD_ROOT}/out/Release/zstd_custom.lib
)

target_compile_features(workload PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(workload PRIVATE /O2 /EHsc)
endif()
