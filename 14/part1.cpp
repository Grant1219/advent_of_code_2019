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

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide the input data filename" << std::endl;
        return 1;
    }

    std::ifstream data(argv[1]);

    std::map<std::string, Recipe> recipes;
    std::map<std::string, int> leftovers;

    std::string line;
    while (std::getline(data, line)) {
        auto recipe = parse_line(line);
        recipes.insert({ recipe.first, recipe.second });
        leftovers.insert({ recipe.first, 0 });
    }

    // populate the first list of requirements with the elements needed for FUEL
    const auto& fuel = recipes.find("FUEL");
    std::vector<Requirement> cur_reqs(fuel->second.ingredients);
    int total_ore = 0;

    for (unsigned int i = 0; i < cur_reqs.size(); ++i) {
        auto& req = cur_reqs[i];

        std::cout << req.second << " " << req.first << " requires..." << std::endl;
        // get the recipe for the currently needed element
        auto new_recipe = recipes.find(req.first);

        // if there is enough to meet the full requirement, use it
        if (leftovers[req.first] >= req.second) {
            std::cout << "Using " << req.second << " leftover " << req.first << std::endl;
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
            std::cout << extra << " " << req.first << " leftover" << std::endl;
        }

        // if the leftover can reduce the amount of batches needed, use a portion
        if (leftovers[req.first] > 0) {
            int extra_batch = leftovers[req.first] / new_recipe->second.count;
            int extra_element = extra_batch * new_recipe->second.count;
            if (extra_batch > 0) {
                // reduce the number of batches to make
                multiplier -= extra_batch;
                leftovers[req.first] -= extra_element;
                std::cout << "Using " << extra_element << " leftover " << req.first << std::endl;
                std::cout << "Still making " << new_recipe->second.count * multiplier << " " << req.first << std::endl;
            }
        }

        // iterate over the next recipe ingredients and apply the multiplier
        for (const auto& ing : new_recipe->second.ingredients) {
            if (ing.first == "ORE") {
                // if an ORE requirement is found just add it to the total required
                total_ore += ing.second * multiplier;
                std::cout << ing.second * multiplier << " ORE" << std::endl;
                continue;
            }

            // try to add the required amount to an existing requirement
            // but only if it exists after the current requirement
            bool found = false;
            for (unsigned int i2 = i + 1; i2 < cur_reqs.size(); ++i2) {
                if (cur_reqs[i2].first == ing.first) {
                    std::cout << ing.second * multiplier << " more " << ing.first << std::endl;
                    cur_reqs[i2].second += (ing.second * multiplier);
                    found = true;
                }
            }

            // otherwise add a new element requirement
            if (!found) {
                std::cout << ing.second * multiplier << " new " << ing.first << std::endl;
                cur_reqs.push_back({ing.first, ing.second * multiplier});
            }
        }
    }

    for (const auto& l : leftovers) {
        std::cout << l.second << " " << l.first << " extra" << std:: endl;
    }

    std::cout << "Total ORE required for 1 FUEL: " << total_ore << std::endl;

    return 0;
}
