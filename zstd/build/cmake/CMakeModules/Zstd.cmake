if (TARGET zstd_custom)
    return()
endif()
 
message(STATUS "Configuring custom ZSTD build")
 
file(GLOB ZSTD_CUSTOM_SOURCES
    ${ZSTD_SOURCE_DIR}/lib/common/*.c
    ${ZSTD_SOURCE_DIR}/lib/compress/*.c
    ${ZSTD_SOURCE_DIR}/lib/decompress/*.c
    ${ZSTD_SOURCE_DIR}/lib/dictBuilder/*.c
)
 
add_library(zstd_custom STATIC ${ZSTD_CUSTOM_SOURCES})
 
target_include_directories(zstd_custom
    PUBLIC
        ${ZSTD_SOURCE_DIR}/lib
        ${ZSTD_SOURCE_DIR}/lib/common
)
 
target_compile_definitions(zstd_custom PRIVATE ZSTD_MULTITHREAD)
 
if (MSVC)
    target_compile_options(zstd_custom PRIVATE /O2)
else()
    target_compile_options(zstd_custom PRIVATE -O3 -Wall)
endif()