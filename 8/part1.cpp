#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int count_layer_digits(char digit, const std::string& layer) {
    int count = 0;
    for (char c : layer) {
        if (c == digit) {
            count++;
        }
    }

    return count;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide the input data filename" << std::endl;
        return 1;
    }

    std::ifstream data(argv[1]);

    std::vector<std::string> layers;

    size_t layer_size = 25 * 6;
    std::string layer(layer_size, '\0');
    while (data.read(&layer[0], layer_size)) {
        layers.push_back(layer);
    }

    std::cout << "Read " << layers.size() << " layers" << std::endl;

    int num_zeros = 99999;
    std::string least_zero_layer;

    for (const auto& layer : layers) {
        int count = count_layer_digits('0', layer);
        if (count < num_zeros) {
            num_zeros = count;
            least_zero_layer = layer;
        }
    }

    std::cout << "Layer with least zeros has: " << num_zeros << std::endl;

    int num_ones = count_layer_digits('1', least_zero_layer);
    int num_twos = count_layer_digits('2', least_zero_layer);

    std::cout << "Ones multiplied by twos is: " << num_ones * num_twos << std::endl;

    return 0;
}
