// day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>

int main()
{
    std::string file_name = "day1.txt";
    std::cout << "Advent of Code 2022\nDay 1 -  Part 1." << std::endl;
    std::ifstream infile(file_name, std::ifstream::in);
    std::string line;
    std::vector<int> totals;
    int amount = 0, total = 0, max_total = 0;
    while (getline(infile, line)) {
        if (sscanf_s(line.c_str(), "%d", &amount) < 1) {
            if (max_total < total) {
                max_total = total;
            }
            totals.push_back(total);
            total = 0;
            continue;
        }
        total += amount;
    }
    infile.close();
    std::cout << "Elf with max calories is carrying: " << max_total << std::endl;

    std::cout << "Part 2." << std::endl;
    int top3_total = 0;
    std::sort(totals.begin(), totals.end(), std::greater<int>());
    if (totals.size() < 3) {
        std::cout << "Not enough elves!" << std::endl;
    }
    else {
        top3_total = totals[0] + totals[1] + totals[2];
        std::cout << "Total of top 3 elves = " << top3_total << std::endl;
    }
    return 0;
}
