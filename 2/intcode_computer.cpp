#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum {
    OP_ADD = 1,
    OP_MUL = 2,
    OP_EXIT = 99
};

std::vector<int> parse_program(std::ifstream& data) {
    std::vector<int> program;
    std::string token;

    while (std::getline(data, token, ',')) {
        program.push_back(std::stoi(token));
    }

    return program;
}

void print_state(int instruction_ptr, const std::vector<int> program) {
    std::cout << instruction_ptr << " : [ ";
    for (const auto& v : program) {
        std::cout << v << ", ";
    }
    std::cout << "]" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Please provide the input data file, and two numbers" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::ifstream data(input_filename);

    auto program = parse_program(data);

    int noun = std::stoi(argv[2]);
    int verb = std::stoi(argv[3]);
    program[1] = noun;
    program[2] = verb;

    // the program always starts at the first opcode
    int instruction_ptr = 0;
    bool done = false;
    bool debug = false;

    while (!done) {
        if (debug)
            print_state(instruction_ptr, program);

        int opcode = program[instruction_ptr];

        if (opcode == OP_ADD || opcode == OP_MUL) {
            // <opcode> <a position> <b position> <result position>
            int a = program[instruction_ptr + 1];
            int b = program[instruction_ptr + 2];
            int dest = program[instruction_ptr + 3];

            if (opcode == OP_ADD) {
                program[dest] = program[a] + program[b];
            }
            else if (opcode == OP_MUL) {
                program[dest] = program[a] * program[b];
            }
        }
        else if (opcode == OP_EXIT) {
            done = true;
        }
        else {
            std::cout << "Error, unknown opcode=" << opcode << " at position=" << instruction_ptr << std::endl;
        }

        // the next opcode is always four positions away
        instruction_ptr += 4;
    }

    std::cout << "Output: " << program[0] << std::endl;
    std::cout << "Program terminated" << std::endl;

    return 0;
}
