#ifndef INTCODE_COMPUTER_HPP
#define INTCODE_COMPUTER_HPP

#include <vector>
#include <queue>

enum {
    STATE_RUN = 0,
    STATE_HALT = 1,
    STATE_INPUT = 2,
    STATE_ERROR = 3
};

class IntcodeComputer {
    public:
        IntcodeComputer(const std::vector<int>& program);

        std::queue<int> execute();
        void reset();
        void set_instruction_ptr(int loc);
        unsigned int get_state() { return state; }

        void push_input(int n) { inputs.push(n); }

    private:
        std::vector<int> initial_program, memory;
        std::queue<int> inputs;
        unsigned int instruction_ptr = 0;
        unsigned int state = STATE_RUN;
};

#endif
