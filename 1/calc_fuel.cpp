#include <string>
#include <fstream>
#include <iostream>

int calc_fuel(int mass) {
    int fuel = (mass / 3) - 2;
    return (fuel > 0) ? fuel : 0;
}

int calc_module_fuel(int module_mass) {
    // the module needs fuel
    int module_fuel = calc_fuel(module_mass);

    // the fuel added also needs fuel, until we reach zero
    int extra_fuel = calc_fuel(module_fuel);
    do {
        module_fuel += extra_fuel;
        extra_fuel = calc_fuel(extra_fuel);
    } while (extra_fuel != 0);

    return module_fuel;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide the input data filename" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::ifstream data(input_filename);

    int total_fuel = 0;
    for (std::string line; std::getline(data, line); ) {
        int mass = std::stoi(line);
        total_fuel += calc_module_fuel(mass);
    }

    std::cout << "Total fuel required: " << total_fuel << std::endl;

    return 0;
}
