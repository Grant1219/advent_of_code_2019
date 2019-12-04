#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

typedef std::pair<int, int> Vec2;

Vec2 get_vector(std::string move) {
    char dir = move[0];
    int steps = std::stoi(move.substr(1));

    if (dir == 'U') {
        return Vec2(0, -steps);
    }
    else if (dir == 'D') {
        return Vec2(0, steps);
    }
    else if (dir == 'L') {
        return Vec2(-steps, 0);
    }
    else if (dir == 'R') {
        return Vec2(steps, 0);
    }
    else {
        std::cout << "Error, unknown movement" << std::endl;
        return Vec2(0, 0);
    }
}

std::vector<std::string> tokenize(const std::string &str, char delim) {
    size_t start;
    size_t end = 0;
    std::vector<std::string> out;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }

    return out;
}

std::vector<Vec2> parse_moves(const std::string& data) {
    std::vector<Vec2> moves;
    std::string token;

    auto str_moves = tokenize(data, ',');
    for (auto str : str_moves) {
        moves.push_back(get_vector(str));
    }

    return moves;
}

int calc_dist_from_origin(const Vec2& w) {
    if (w.first == 0 && w.second == 0)
        return 1410065408;

    return std::abs(w.first + w.second);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide the input data filename" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::ifstream data(input_filename);

    // split up the text lines into actual 2d vectors
    std::string line;
    std::getline(data, line);
    auto first_moves = parse_moves(line);
    std::getline(data, line);
    auto second_moves = parse_moves(line);

    Vec2 wire1 = { 0, 0};
    Vec2 wire2 = { 0, 0};
    std::map<Vec2, bool> wire1_points;
    std::map<Vec2, bool> wire2_points;
    int closest_dist = 1410065408;

    auto first_itr = first_moves.begin();
    auto second_itr = second_moves.begin();

    while (first_itr != first_moves.end() && second_itr != second_moves.end()) {
        // iterate over every position in the vector and add a point
        if (first_itr->first != 0) {
            for (int i = 0; i < std::abs(first_itr->first); ++i) {
                wire1_points.insert({ Vec2(wire1.first, wire1.second), true });

                if (first_itr->first > 0)
                    wire1.first += 1;
                else
                    wire1.first -= 1;
            }
        }
        else if (first_itr->second != 0) {
            for (int i = 0; i < std::abs(first_itr->second); ++i) {
                wire1_points.insert({ Vec2(wire1.first, wire1.second), true });

                if (first_itr->second > 0)
                    wire1.second += 1;
                else
                    wire1.second -= 1;
            }
        }

        // now iterate over the vector in the second wire while searching for intersections
        if (second_itr->first != 0) {
            for (int i = 0; i < std::abs(second_itr->first); ++i) {
                Vec2 v = Vec2(wire2.first, wire2.second);
                wire2_points.insert({ v, true });

                // only search if we are closer than the last closest distance
                int dist = calc_dist_from_origin(v);
                if (dist < closest_dist) {
                    auto itr = wire1_points.find(v);
                    if (itr != wire1_points.end())
                        closest_dist = dist;
                }

                if (second_itr->first > 0)
                    wire2.first += 1;
                else
                    wire2.first -= 1;
            }
        }
        else if (second_itr->second != 0) {
            for (int i = 0; i < std::abs(second_itr->second); ++i) {
                Vec2 v = Vec2(wire2.first, wire2.second);
                wire2_points.insert({ v, true });

                // only search if we are closer than the last closest distance
                int dist = calc_dist_from_origin(v);
                if (dist < closest_dist) {
                    auto itr = wire1_points.find(v);
                    if (itr != wire1_points.end())
                        closest_dist = dist;
                }

                if (second_itr->second > 0)
                    wire2.second += 1;
                else
                    wire2.second -= 1;
            }
        }

        first_itr++;
        second_itr++;
    }

    std::cout << "The closest intersection is: " << closest_dist << std::endl;

    return 0;
}
