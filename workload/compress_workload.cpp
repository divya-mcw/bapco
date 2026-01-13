#include <zstd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <string>

static std::vector<char> readFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Failed to open input file");

    size_t size = (size_t)file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

static void writeFile(const std::string& filename,
                      const std::vector<char>& data,
                      size_t size) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to write output file");
    out.write(data.data(), size);
}

void run_compress(const char* inputFile) {
    auto input = readFile(inputFile);
    size_t inputSize = input.size();

    std::cout << "Input size: " << inputSize << " bytes\n";
    std::cout << "Level,CompressedSize,Ratio,Time_us\n";

    int maxLevel = ZSTD_maxCLevel();

    for (int level = 1; level <= maxLevel; level++) {
        size_t maxCompressedSize = ZSTD_compressBound(inputSize);
        std::vector<char> compressed(maxCompressedSize);

        auto t1 = std::chrono::high_resolution_clock::now();
        size_t compressedSize = ZSTD_compress(
            compressed.data(),
            maxCompressedSize,
            input.data(),
            inputSize,
            level
        );
        auto t2 = std::chrono::high_resolution_clock::now();

        if (ZSTD_isError(compressedSize)) {
            std::cerr << "Compression error at level "
                      << level << ": "
                      << ZSTD_getErrorName(compressedSize) << "\n";
            continue;
        }

        auto timeUs =
            std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::string outName =
            "image_level_" + std::to_string(level) + ".tar.zst";
        writeFile(outName, compressed, compressedSize);

        std::cout << level << ","
                  << compressedSize << ","
                  << (double)compressedSize / inputSize << ","
                  << timeUs << "\n";
    }
}