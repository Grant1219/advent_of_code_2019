#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using Requirement = std::pair<std::string, int>;

struct Recipe {
    Recipe(Requirement req) : element(req.first), count(req.second) {}

    std::string element;
    int count;
    std::vector<Requirement> ingredients;
};

using RecipeMap = std::map<std::string, Recipe>;
using LeftoverMap = std::map<std::string, int>;

Requirement parse_req(std::string req) {
    std::string::size_type pos;
    pos = req.find(" ");

    int amount = std::stoi(req.substr(0, pos));
    std::string name = req.substr(pos + 1);

    return std::make_pair(name, amount);
}

std::pair<std::string, Recipe> parse_line(std::string line) {
    std::string::size_type pos;
    pos = line.find("=>");

    Recipe result(parse_req(line.substr(pos + 3)));

    std::string elements = line.substr(0, pos - 1);
    std::string::size_type last_pos = 0;

    while (pos != std::string::npos) {
        pos = elements.find(",", last_pos);

        if (pos != std::string::npos) {
            result.ingredients.emplace_back(parse_req(elements.substr(last_pos, pos - last_pos)));
            last_pos = pos + 2;
        }
        else {
            result.ingredients.emplace_back(parse_req(elements.substr(last_pos)));
        }
    }

    return std::make_pair(result.element, result);
}

bool create_one_fuel(const RecipeMap& recipes, LeftoverMap& leftovers, uint64_t& cur_ore) {
    // populate the first list of requirements with the elements needed for FUEL
    const auto& fuel = recipes.find("FUEL");
    std::vector<Requirement> cur_reqs(fuel->second.ingredients);
    uint64_t used_ore = 0;

    for (unsigned int i = 0; i < cur_reqs.size(); ++i) {
        auto& req = cur_reqs[i];

        // get the recipe for the currently needed element
        auto new_recipe = recipes.find(req.first);

        // if there is enough to meet the full requirement, use it
        if (leftovers[req.first] >= req.second) {
            leftovers[req.first] -= req.second;
            continue;
        }

        // divide the recipe count into the actual count required
        int multiplier = req.second / new_recipe->second.count;
        // if it doesn't divide evenly the result will be short, so add one more portion
        if (multiplier * new_recipe->second.count < req.second) {
            ++multiplier;

            // now there will be leftovers, so record them
            int extra = multiplier * new_recipe->second.count - req.second;
            leftovers[req.first] += extra;
        }

        // if the leftover can reduce the amount of batches needed, use a portion
        if (leftovers[req.first] > 0) {
            int extra_batch = leftovers[req.first] / new_recipe->second.count;
            int extra_element = extra_batch * new_recipe->second.count;
            if (extra_batch > 0) {
                // reduce the number of batches to make
                multiplier -= extra_batch;
                leftovers[req.first] -= extra_element;
            }
        }

        // iterate over the next recipe ingredients and apply the multiplier
        for (const auto& ing : new_recipe->second.ingredients) {
            if (ing.first == "ORE") {
                // if an ORE requirement is found just add it to the total required
                used_ore += ing.second * multiplier;
                continue;
            }

            // try to add the required amount to an existing requirement
            // but only if it exists after the current requirement
            bool found = false;
            for (unsigned int i2 = i + 1; i2 < cur_reqs.size(); ++i2) {
                if (cur_reqs[i2].first == ing.first) {
                    cur_reqs[i2].second += (ing.second * multiplier);
                    found = true;
                }
            }

            // otherwise add a new element requirement
            if (!found) {
                cur_reqs.push_back({ing.first, ing.second * multiplier});
            }
        }
    }

    if (used_ore <= cur_ore) {
        cur_ore -= used_ore;
        return true;
    }
    else {
        return false;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide the input data filename" << std::endl;
        return 1;
    }

    std::ifstream data(argv[1]);

    RecipeMap recipes;
    LeftoverMap leftovers;

    std::string line;
    while (std::getline(data, line)) {
        auto recipe = parse_line(line);
        recipes.insert({ recipe.first, recipe.second });
        leftovers.insert({ recipe.first, 0 });
    }

    uint64_t cur_ore = 1000000000000;
    uint32_t total_fuel = 0;

    while (create_one_fuel(recipes, leftovers, cur_ore)) {
        ++total_fuel;
    }

    std::cout << "FUEL created from 1 trillion ORE: " << total_fuel << std::endl;

    return 0;
}
