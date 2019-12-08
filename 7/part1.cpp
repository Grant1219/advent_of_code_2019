#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "intcode_computer.hpp"

std::vector<int> parse_program(std::ifstream& data) {
    std::vector<int> program;
    std::string token;

    while (std::getline(data, token, ',')) {
        program.push_back(std::stoi(token));
    }

    return program;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Please provide the digits data and intcode program" << std::endl;
        return 1;
    }

    std::cout << "Reading digits from: " << argv[1] << std::endl;
    std::ifstream digits_data(argv[1]);

    std::vector<std::vector<int>> inputs;
    for (std::string line; std::getline(digits_data, line); ) {
        std::vector<int> digits;

        for (int i = 0; i < 5; ++i) {
            digits.push_back(line[i] - 48);
        }

        inputs.push_back(digits);
    }

    std::cout << "Reading intcode program from: " << argv[2] << std::endl;
    std::ifstream program_data(argv[2]);

    auto intcomp = IntcodeComputer(parse_program(program_data));

    int highest_signal = 0;
    for (const auto digits : inputs) {
        int signal = 0;

        for (const auto phase : digits) {
            // run the program for each digit
            // provide the output to the inputs of the next
            intcomp.push_input(phase);
            intcomp.push_input(signal);
            auto outputs = intcomp.execute();
            signal = outputs.front();
            intcomp.reset();
        }

        std::cout << "Signal result: " << signal << std::endl;

        if (signal > highest_signal) {
            highest_signal = signal;
        }
    }

    std::cout << "The highest signal possible is: " << highest_signal << std::endl;

    return 0;
}
