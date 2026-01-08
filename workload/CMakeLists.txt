cmake_minimum_required(VERSION 3.15)

project(zstd_workload LANGUAGES CXX)

# Path to ZSTD
set(ZSTD_ROOT "${CMAKE_SOURCE_DIR}/../zstd")

# Create executable
add_executable(workload workload.cpp)

# Headers
target_include_directories(workload PRIVATE
    ${ZSTD_ROOT}/lib
)

# Link your custom-built library
target_link_libraries(workload PRIVATE
    ${ZSTD_ROOT}/out/Release/zstd_custom.lib
)

# Runtime & language settings
target_compile_features(workload PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(workload PRIVATE /EHsc /MD)
endif()

 