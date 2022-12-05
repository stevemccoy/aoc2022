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

int score_round(char them, char us) {
    switch (us)
    {
    case 'X':
        return 1 + ((them == 'A') ? 3 : ((them == 'B') ? 0 : 6));
    case 'Y':
        return 2 + ((them == 'A') ? 6 : ((them == 'B') ? 3 : 0));
    case 'Z':
        return 3 + ((them == 'A') ? 0 : ((them == 'B') ? 6 : 3));
    default:
        return -1;
    }
}

char choose_play(char them, char strategy) {
    switch (strategy)
    {
    case 'X':   // Lose.
        return ((them == 'A') ? 'Z' : ((them == 'B') ? 'X' : 'Y'));
    case 'Y':   // Draw.
        return ((them == 'A') ? 'X' : ((them == 'B') ? 'Y' : 'Z'));
    case 'Z':   // Win.
        return ((them == 'A') ? 'Y' : ((them == 'B') ? 'Z' : 'X'));
    default:
        return ' ';
    }
}

int main()
{
    std::string file_name = "day2.txt";
    std::cout << "Advent of Code 2022\nDay 2 -  Part 1." << std::endl;
    auto lines = read_input_file(file_name.c_str());
    char us = ' ', them = ' ';
    int score = 0, total = 0;

    // Us = X => Rock, Y => Paper, Z => Scissors.
    for (auto line : lines) {
        sscanf_s(line.c_str(), "%c %c", &them, 1, &us, 1);
        score = score_round(them, us);
        if (score < 0) {
            std::cerr << "Error in input: " << line << std::endl;
            return 1;
        }
        total += score;
    }
    std::cout << "Total score = " << total << std::endl;

    std::cout << "Part 2." << std::endl;
    // X => Lose, Y => Draw, Z => Win
    total = score = 0;
    char need = ' ';
    for (auto line : lines) {
        sscanf_s(line.c_str(), "%c %c", &them, 1, &need, 1);
        us = choose_play(them, need);
        score = score_round(them, us);
        if (score < 0) {
            std::cerr << "Error in input: " << line << std::endl;
            return 1;
        }
        total += score;
    }
    std::cout << "Total score = " << total << std::endl;
    return 0;
}
