#include <zstd.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

static std::vector<char> readFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open input file");
    }

    size_t size = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: workload <input_file>\n";
        return 1;
    }

    auto input = readFile(argv[1]);
    size_t inputSize = input.size();

    size_t maxCompressedSize = ZSTD_compressBound(inputSize);
    std::vector<char> compressed(maxCompressedSize);

    auto t1 = std::chrono::high_resolution_clock::now();
    size_t compressedSize = ZSTD_compress(
        compressed.data(),
        maxCompressedSize,
        input.data(),
        inputSize,
        3  // compression level
    );
    auto t2 = std::chrono::high_resolution_clock::now();

    if (ZSTD_isError(compressedSize)) {
        std::cerr << "Compression error: "
                  << ZSTD_getErrorName(compressedSize) << "\n";
        return 1;
    }

    std::vector<char> decompressed(inputSize);

    auto t3 = std::chrono::high_resolution_clock::now();
    size_t decompressedSize = ZSTD_decompress(
        decompressed.data(),
        inputSize,
        compressed.data(),
        compressedSize
    );
    auto t4 = std::chrono::high_resolution_clock::now();

    if (ZSTD_isError(decompressedSize)) {
        std::cerr << "Decompression error\n";
        return 1;
    }

    auto compressTime =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto decompressTime =
        std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    std::cout << "Input size        : " << inputSize << " bytes\n";
    std::cout << "Compressed size   : " << compressedSize << " bytes\n";
    std::cout << "Compression ratio : "
              << (double)compressedSize / inputSize << "\n";
    std::cout << "Compression time  : " << compressTime << " us\n";
    std::cout << "Decompression time: " << decompressTime << " us\n";

    return 0;
}
 