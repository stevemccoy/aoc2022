#include <cstdio>

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include<fstream>
#include<string>
#include<strstream>

int num_rows, num_cols;

std::vector<std::string> read_input_file(const char* file_name) {
    std::vector<std::string> result;
    std::ifstream infile(file_name, std::ifstream::in);
    std::string line;
    while (getline(infile, line)) {
        result.push_back(line);
    }
    infile.close();
    return result;
}

void setup_wood(const std::vector<std::string>& lines) {
    num_rows = lines.size();
    num_cols = lines[0].size();
    if (wood != nullptr) {
        delete[] wood;
        delete[] vis_grid;
        wood = nullptr;
        vis_grid = nullptr;
    }

    int num_chars = num_cols * num_rows * sizeof(char);
    wood = new char[num_chars];
    vis_grid = new char[num_chars];
    memset(wood, 0, num_chars);
    memset(vis_grid, 'f', num_chars);

    int r = 0, c = 0;
    for (auto line : lines) {
        c = 0;
        for (auto ch : line) {
            set(c++, r, ch - '0');
        }
        r++;
    }
}

int main()
{
    std::cout << "Advent of Code 2022\nDay 12 -  Part 1." << std::endl;
    auto inputLines = read_input_file("test12.txt");
}
