#pragma once
#include <fstream>
#include <sstream>
#include <string>

class Config {
public:
    int read_delay_ms = 0;
    int write_delay_ms = 0;
    int move_delay_ms = 0;
    size_t m = 4096;
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return false;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '=')) {
                std::string value;
                if (std::getline(iss, value)) {
                    if (key == "read_delay_ms") {
                        read_delay_ms = std::stoi(value);
                    }
                    else if (key == "write_delay_ms") {
                        write_delay_ms = std::stoi(value);
                    }
                    else if (key == "move_delay_ms") {
                        move_delay_ms = std::stoi(value);
                    }
                    else if (key == "memory_limit_bytes") {
                        m = std::stoul(value) / sizeof(int);
                        if (m == 0) m = 1;
                    }
                }
            }
        }
        return true;
    }
};
