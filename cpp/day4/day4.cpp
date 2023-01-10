#include <iostream>
#include <vector>
#include<fstream>
#include<string>
#include<algorithm>

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

bool interval_contains(int from_big, int to_big, int from_small, int to_small) {
    return ((from_big <= from_small) && (to_big >= to_small));
}

bool interval_overlaps(int from1, int to1, int from2, int to2) {
    return (((from2 <= to1) && (from2 >= from1)) || ((from1 <= to2) && (from1 >= from2)));
}

int main()
{
    std::string file_name = "day4.txt";
    std::cout << "Advent of Code 2022\nDay 4 -  Part 1." << std::endl;
    auto lines = read_input_file(file_name.c_str());
    int from1, to1, from2, to2;
    int count = 0;
    for (auto line : lines) {
        sscanf_s(line.c_str(), "%d-%d,%d-%d", &from1, &to1, &from2, &to2);
        if (interval_contains(from1, to1, from2, to2) ||
            interval_contains(from2, to2, from1, to1)) {
            count++;
        }
    }
    std::cout << "Count of contained intervals = " << count << std::endl;

    std::cout << "Part 2." << std::endl;
    count = 0;
    for (auto line : lines) {
        sscanf_s(line.c_str(), "%d-%d,%d-%d", &from1, &to1, &from2, &to2);
        if (interval_overlaps(from1, to1, from2, to2)) {
            count++;
        }
    }
    std::cout << "Count of overlapping intervals = " << count << std::endl;

    return 0;
}
