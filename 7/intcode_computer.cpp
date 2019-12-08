#include <iostream>

#include "intcode_computer.hpp"

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

IntcodeComputer::IntcodeComputer(const std::vector<int>& program)
    : initial_program(program), memory(program) {}

std::queue<int> IntcodeComputer::execute() {
    unsigned int incr = 0;
    std::queue<int> outputs;
    state = STATE_RUN;

    while (true) {
        int opcode = memory[instruction_ptr];

        // chop off parameter mode digits
        int param3_mode = opcode / 10000;
        opcode = opcode % 10000;
        int param2_mode = opcode / 1000;
        opcode = opcode % 1000;
        int param1_mode = opcode / 100;
        opcode = opcode % 100;

        if (opcode == OP_EXIT) {
            state = STATE_HALT;
            break;
        }
        else if (opcode == OP_IN) {
            std::cout << "IN " << memory[instruction_ptr + 1] << std::endl;

            if (inputs.size() > 0) {
                std::cout << "> " << inputs.front() << std::endl;
                memory[memory[instruction_ptr + 1]] = inputs.front();
                inputs.pop();
            }
            else {
                std::cout << "Waiting for input" << std::endl;
                state = STATE_INPUT;
                break;
            }

            incr = 2;
        }
        else if (opcode == OP_OUT) {
            std::cout << "OUT " << memory[instruction_ptr + 1] << std::endl;
            std::cout << memory[memory[instruction_ptr + 1]] << std::endl;

            outputs.push(memory[memory[instruction_ptr + 1]]);
            incr = 2;
        }
        else {
            int param1, param2, param3 = 0;
            param3 = memory[instruction_ptr + 3];

            // check parameter modes
            if (param1_mode == IMMEDIATE)
                param1 = memory[instruction_ptr + 1];
            else
                param1 = memory[memory[instruction_ptr + 1]];

            if (param2_mode == IMMEDIATE)
                param2 = memory[instruction_ptr + 2];
            else
                param2 = memory[memory[instruction_ptr + 2]];

            if (opcode == OP_ADD) {
                std::cout << "ADD " << param1 << " + " << param2 << " -> " << param3 << std::endl;
                memory[param3] = param1 + param2;
                incr = 4;
            }
            else if (opcode == OP_MUL) {
                std::cout << "MUL " << param1 << " " << param2 << "-> " << param3 << std::endl;
                memory[param3] = param1 * param2;
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
                    memory[param3] = 1;
                else
                    memory[param3] = 0;

                incr = 4;
            }
            else if(opcode == OP_CEQ) {
                std::cout << "JIF " << param1 << " == " << param2 << " -> " << param3 << std::endl;
                if (param1 == param2)
                    memory[param3] = 1;
                else
                    memory[param3] = 0;

                incr = 4;
            }
            else {
                std::cout << "Error, unknown opcode=" << opcode << " at position=" << instruction_ptr << std::endl;
                state = STATE_ERROR;
                break;
            }
        }

        instruction_ptr += incr;
    }

    if (state == STATE_HALT)
        std::cout << "Program terminated successfully" << std::endl;
    else if (state == STATE_ERROR)
        std::cout << "Program terminated with errors" << std::endl;

    return outputs;
}

void IntcodeComputer::reset() {
    memory = initial_program;
    instruction_ptr = 0;
    state = STATE_RUN;

    while (!inputs.empty()) {
        inputs.pop();
    }

    std::cout << "Program memory reset" << std::endl;
}

void IntcodeComputer::set_instruction_ptr(int loc) {
    if (loc > 0 && loc < memory.size()) {
        std::cout << "Set instruction pointer to: " << loc << std::endl;
        instruction_ptr = loc;
    }
}
