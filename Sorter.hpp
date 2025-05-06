#pragma once
#include <vector>
#include <algorithm>
#include <cstdio>
#include "Config.hpp"
#include "Tape.hpp"

struct Run {
    Tape* tape;
    int value;
    bool hasValue;
};

class Sorter {
    Config config;
public:
    Sorter(const Config& config)
        : config(config)
    {
    }

    void mySort(const std::string& inputTape, const std::string& outputTape) {
        std::vector<std::string> runFiles;
        split(inputTape, runFiles);
        merge(runFiles, outputTape);
        for (const auto& f : runFiles) std::remove(f.c_str());
    }

    void split(const std::string& inputTape, std::vector<std::string>& runFiles) {
        Tape tape(inputTape, config);
        tape.rewind();
        int value;
        size_t runIdx = 0;
        while (!tape.eof()) {
            std::vector<int> buffer;
            buffer.reserve(config.m);
            for (size_t i = 0; i < config.m && !tape.eof(); ++i) {
                if (tape.read(value)) {
                    buffer.push_back(value);
                    tape.moveRight();
                }
            }
            if (buffer.empty()) break;
            std::sort(buffer.begin(), buffer.end());
            std::string runFile = "run_" + std::to_string(runIdx++) + ".bin";
            runFiles.push_back(runFile);
            std::ofstream out(runFile, std::ios::binary);
            for (int v : buffer) {
                out.write(reinterpret_cast<const char*>(&v), sizeof(int));
            }
        }
    }

    void merge(const std::vector<std::string>& runFiles, const std::string& outputTape) {
        std::vector<Run> runs;
        for (const auto& file : runFiles) {
            Tape* t = new Tape(file, config);
            t->rewind();
            int val;
            bool ok = t->read(val);
            runs.push_back(Run{ t, val, ok });
        }

        Tape outTape(outputTape, config);

        while (true) {
            int minIdx = -1;
            int minVal = 0;
            for (size_t i = 0; i < runs.size(); ++i) {
                if (runs[i].hasValue) {
                    minIdx = i;
                    minVal = runs[i].value;
                    break;
                }
            }
            if (minIdx == -1) break;
            for (size_t i = minIdx + 1; i < runs.size(); ++i) {
                if (runs[i].hasValue && runs[i].value < minVal) {
                    minIdx = i;
                    minVal = runs[i].value;
                }
            }
            outTape.write(runs[minIdx].value);
            outTape.moveRight();
            if (runs[minIdx].tape->moveRight() && runs[minIdx].tape->read(runs[minIdx].value)) {
                runs[minIdx].hasValue = true;
            }
            else {
                runs[minIdx].hasValue = false;
            }
        }
        for (auto& r : runs) {
            delete r.tape;
        }
    }

};
