#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum {
    OP_ADD = 1,
    OP_MUL = 2,
    OP_IN = 3,
    OP_OUT = 4,
    OP_JIT = 5,
    OP_JIF = 6,
    OP_CLT = 7,
    OP_CEQ = 8,
    OP_EXIT = 99
};

enum {
    POSITION = 0,
    IMMEDIATE = 1
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
    if (argc < 2) {
        std::cout << "Please provide the input data file" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::ifstream data(input_filename);

    auto program = parse_program(data);

    // the program always starts at the first opcode
    int instruction_ptr = 0;
    int incr = 0;
    bool done = false;
    bool debug = false;

    while (!done) {
        if (debug)
            print_state(instruction_ptr, program);

        int opcode = program[instruction_ptr];

        // chop off parameter mode digits
        int param3_mode = opcode / 10000;
        opcode = opcode % 10000;
        int param2_mode = opcode / 1000;
        opcode = opcode % 1000;
        int param1_mode = opcode / 100;
        opcode = opcode % 100;

        if (opcode == OP_EXIT) {
            done = true;
        }
        else if (opcode == OP_IN) {
            std::cout << "IN " << program[instruction_ptr + 1] << std::endl;
            int value;
            std::cout << "> ";
            std::cin >> value;
            program[program[instruction_ptr + 1]] = value;
            incr = 2;
        }
        else if (opcode == OP_OUT) {
            std::cout << "OUT " << program[instruction_ptr + 1] << std::endl;
            std::cout << program[program[instruction_ptr + 1]] << std::endl;
            incr = 2;
        }
        else {
            int param1, param2, param3 = 0;
            param3 = program[instruction_ptr + 3];

            // check parameter modes
            if (param1_mode == IMMEDIATE)
                param1 = program[instruction_ptr + 1];
            else
                param1 = program[program[instruction_ptr + 1]];

            if (param2_mode == IMMEDIATE)
                param2 = program[instruction_ptr + 2];
            else
                param2 = program[program[instruction_ptr + 2]];

            if (opcode == OP_ADD) {
                std::cout << "ADD " << param1 << " + " << param2 << " -> " << param3 << std::endl;
                program[param3] = param1 + param2;
                incr = 4;
            }
            else if (opcode == OP_MUL) {
                std::cout << "MUL " << param1 << " " << param2 << "-> " << param3 << std::endl;
                program[param3] = param1 * param2;
                incr = 4;
            }
            else if(opcode == OP_JIT) {
                std::cout << "JIT " << param1 << " -> " << param2 << std::endl;
                if (param1 != 0) {
                    instruction_ptr = param2;
                    incr = 0;
                }
                else
                    incr = 3;
            }
            else if(opcode == OP_JIF) {
                std::cout << "JIF " << param1 << " -> " << param2 << std::endl;
                if (param1 == 0) {
                    instruction_ptr = param2;
                    incr = 0;
                }
                else
                    incr = 3;
            }
            else if(opcode == OP_CLT) {
                std::cout << "JIF " << param1 << " < " << param2 << " -> " << param3 << std::endl;
                if (param1 < param2)
                    program[param3] = 1;
                else
                    program[param3] = 0;

                incr = 4;
            }
            else if(opcode == OP_CEQ) {
                std::cout << "JIF " << param1 << " == " << param2 << " -> " << param3 << std::endl;
                if (param1 == param2)
                    program[param3] = 1;
                else
                    program[param3] = 0;

                incr = 4;
            }
            else {
                std::cout << "Error, unknown opcode=" << opcode << " at position=" << instruction_ptr << std::endl;
                break;
            }
        }

        instruction_ptr += incr;
    }

    if (done)
        std::cout << "Program terminated successfully" << std::endl;
    else
        std::cout << "Program terminated with errors" << std::endl;

    return 0;
}
