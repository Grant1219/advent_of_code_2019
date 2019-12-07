#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

typedef std::vector<std::pair<std::string, std::string>> OrbitVec;

struct Body {
    Body(std::string name, std::shared_ptr<Body> parent)
        : name(name), parent(parent) {}

    std::string name;
    std::shared_ptr<Body> parent;
    std::vector<std::shared_ptr<Body>> children;
};

int insert_bodies(std::shared_ptr<Body> cur, int depth, OrbitVec& orbits) {
    std::cout << "Looking for children of: " << cur->name << std::endl;

    // find current body's children
    while (true) {
        auto itr = std::find_if(orbits.begin(), orbits.end(),
                [&cur](const auto& o) { return o.first == cur->name; });

        if (itr != orbits.end()) {
            std::cout << "Found: " << itr->second << std::endl;
            cur->children.emplace_back(std::make_shared<Body>(itr->second, cur));
            orbits.erase(itr);
        }
        else {
            break;
        }
    }

    // recurse over children and insert all their children
    // while adding up the max depth from each leaf
    int sum_depths = 0;
    for (auto c : cur->children) {
        sum_depths += insert_bodies(c, depth + 1, orbits);
    }

    return depth + sum_depths;
}

std::shared_ptr<Body> find_body(std::string name, std::shared_ptr<Body> cur) {
    if (cur->name == name)
        return cur;

    if (cur->children.size() == 0)
        return nullptr;

    std::shared_ptr<Body> found = nullptr;
    for (const auto body : cur->children) {
        found = find_body(name, body);

        if (found)
            return found;
    }

    return nullptr;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide the input data filename" << std::endl;
        return 1;
    }

    std::ifstream data(argv[1]);

    std::string line;
    std::vector<std::pair<std::string, std::string>> orbits;
    std::pair<std::string, std::string> root_pair;

    while (std::getline(data, line)) {
        size_t sep = line.find(")");
        std::string first_body = line.substr(0, sep);
        std::string second_body = line.substr(sep + 1);

        orbits.push_back(std::make_pair(first_body, second_body));

        if (first_body == "COM") {
            root_pair = orbits.back();
        }
    }

    auto root = std::make_shared<Body>(root_pair.first, nullptr);
    int total_depths = insert_bodies(root, 0, orbits);

    std::cout << "Total depth of all leaf nodes: " << total_depths << std::endl;

    std::shared_ptr<Body> santa = nullptr;
    auto you = find_body("YOU", root);
    int you_transfers = 0;

    while (true) {
        you = you->parent;

        santa = find_body("SAN", you);

        if (santa != nullptr) {
            break;
        }

        you_transfers++;
    }

    int santa_transfers = 0;

    while (true) {
        santa = santa->parent;

        if (santa == you) {
            break;
        }

        santa_transfers++;
    }

    std::cout << "Orbit transfers required: " << you_transfers + santa_transfers << std::endl;

    return 0;
}
