#pragma once
#include <fstream>
#include <thread>
#include "Config.hpp"

class Tape {
    std::fstream file;
    std::string filename;
    Config config;
    size_t pos = 0;
public:
    Tape(const std::string& filename, const Config& config)
        : filename(filename), config(config), pos(0)
    {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            std::ofstream create(filename, std::ios::out | std::ios::binary);
            create.close();
            file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
            if (!file) {
                throw std::runtime_error("Cannot open tape file: " + filename);
            }
        }
    }

    void sleep(int ms) {
        if (ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    bool read(int& value) {
        sleep(config.read_delay_ms);
        file.clear();
        file.seekg(pos * sizeof(int), std::ios::beg);
        return static_cast<bool>(file.read(reinterpret_cast<char*>(&value), sizeof(int)));
    }

    bool write(int value) {
        sleep(config.write_delay_ms);
        file.clear();
        file.seekp(pos * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<const char*>(&value), sizeof(int));
        return file.good();
    }

    bool moveLeft() {
        if (pos == 0) return false;
        sleep(config.move_delay_ms);
        --pos;
        return true;
    }

    bool moveRight() {
        sleep(config.move_delay_ms);
        ++pos;
        return true;
    }

    void rewind() {
        sleep(pos * config.move_delay_ms);
        pos = 0;
    }

    bool eof() {
        file.clear();
        file.seekg(0, std::ios::end);
        size_t fileSize = static_cast<size_t>(file.tellg());
        return (pos * sizeof(int)) >= fileSize;
    }

    ~Tape() {
        file.close();
    }
};
