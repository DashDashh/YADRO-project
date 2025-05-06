#include <iostream>
#include <string>
#include "Tape.hpp"
#include "Config.hpp"
#include "Sorter.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_tape> <output_tape> <config_file>\n";
        return 1;
    }

    std::string inputTapeFile = argv[1];
    std::string outputTapeFile = argv[2];
    std::string configFile = argv[3];

    Config config;
    if (!config.loadFromFile(configFile)) {
        std::cerr << "Failed to load config file: " << configFile << std::endl;
        return 2;
    }

    try {
        Sorter sorter(config);
        sorter.mySort(inputTapeFile, outputTapeFile);
        std::cout << "Done.\n";
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }
}
