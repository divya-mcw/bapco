cmake_minimum_required(VERSION 3.15)
project(zstd_workloads LANGUAGES CXX)

set(ZSTD_ROOT "${CMAKE_SOURCE_DIR}/../zstd")

add_executable(compress_workload compress_workload.cpp)
add_executable(decompress_workload decompress_workload.cpp)

target_include_directories(compress_workload PRIVATE ${ZSTD_ROOT}/lib)
target_include_directories(decompress_workload PRIVATE ${ZSTD_ROOT}/lib)

target_link_libraries(compress_workload PRIVATE
    ${ZSTD_ROOT}/out/Release/zstd_custom.lib
)
target_link_libraries(decompress_workload PRIVATE
    ${ZSTD_ROOT}/out/Release/zstd_custom.lib
)

target_compile_features(compress_workload PRIVATE cxx_std_17)
target_compile_features(decompress_workload PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(compress_workload PRIVATE /O2 /EHsc)
    target_compile_options(decompress_workload PRIVATE /O2 /EHsc)
endif()
