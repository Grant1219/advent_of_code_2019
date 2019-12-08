#include <string>
#include <fstream>
#include <iostream>
#include <vector>

void render_layer(std::string& dest, const std::string& layer) {
    for (int i = 0; i < dest.size(); ++i) {
        // character 0 (black) and 1 (white) will be less than 2 (transparent)
        if (layer[i] < '2') {
            dest[i] = layer[i];
        }
    }
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

    std::string result_layer = layers.back();

    for (auto ritr = layers.crbegin(); ritr != layers.crend(); ++ritr) {
        render_layer(result_layer, (*ritr));
    }

    std::cout << "Result image:" << std::endl;
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 25; ++x) {
            if (result_layer[y * 25 + x] == '0')
                std::cout << ". ";
            else
                std::cout << "# ";
        }
        std::cout << std::endl;
    }

    return 0;
}
