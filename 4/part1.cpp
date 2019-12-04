#include <iostream>
#include <string>

int main(int argc, char** argv) {
    int start = 197487;
    int end = 673251;

    int valid_numbers = 0;

    for (int n = start; n <= end; ++n) {
        std::string nstr = std::to_string(n);

        bool increasing = true;
        bool adjacent = false;
        char prev_c = 0;
        for (int i = 0; i < nstr.size(); ++i) {
            char c = nstr[i];

            // is the character not larger or a repeat, skip
            if (c < prev_c) {
                increasing = false;
                break;
            }

            // check if any adjacent character matches
            if (c == prev_c)
                adjacent = true;

            prev_c = c;
        }

        if (increasing && adjacent) {
            valid_numbers++;
        }
    }

    std::cout << "Numbers matching the rules: " << valid_numbers << std::endl;

    return 0;
}
