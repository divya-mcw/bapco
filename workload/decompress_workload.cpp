#include <zstd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdexcept>

static std::vector<char> readFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Failed to open file");

    size_t size = (size_t)file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

void run_decompress(const char* inputFile) {
    auto compressed = readFile(inputFile);
    size_t compressedSize = compressed.size();

    unsigned long long originalSize =
        ZSTD_getFrameContentSize(compressed.data(), compressedSize);

    if (originalSize == ZSTD_CONTENTSIZE_ERROR ||
        originalSize == ZSTD_CONTENTSIZE_UNKNOWN) {
        throw std::runtime_error("Cannot determine original size");
    }

    std::vector<char> decompressed((size_t)originalSize);

    auto t1 = std::chrono::high_resolution_clock::now();
    size_t decompressedSize = ZSTD_decompress(
        decompressed.data(),
        originalSize,
        compressed.data(),
        compressedSize
    );
    auto t2 = std::chrono::high_resolution_clock::now();

    if (ZSTD_isError(decompressedSize)) {
        throw std::runtime_error(ZSTD_getErrorName(decompressedSize));
    }

    auto timeUs =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << "CompressedSize,DecompressedSize,Time_us\n";
    std::cout << compressedSize << ","
              << decompressedSize << ","
              << timeUs << "\n";
}
