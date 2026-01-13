#include <iostream>
#include <string>

/* function declarations */
void run_compress(const char* inputFile);
void run_decompress(const char* inputFile);

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage:\n"
                  << "  workload compress <input.tar>\n"
                  << "  workload decompress <input.tar.zst>\n";
        return 1;
    }

    std::string mode = argv[1];

    try {
        if (mode == "compress") {
            run_compress(argv[2]);
        }
        else if (mode == "decompress") {
            run_decompress(argv[2]);
        }
        else {
            std::cerr << "Unknown mode: " << mode << "\n";
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << "\n";
        return 2;
    }

    return 0;
}
