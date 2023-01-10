#include <iostream>
#include <vector>
#include<fstream>
#include<string>
#include<algorithm>
#include<set>

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

int priority_score(char ch) {
    if (islower(ch)) {
        return (ch - 'a' + 1);
    }
    if (isupper(ch)) {
        return (ch - 'A' + 27);
    }
    return 0;
}

void inventory(const std::string& line, std::set<char>& seen)
{
    seen.clear();
    for (auto ch : line) {
        seen.insert(ch);
    }
}

char common_item(const std::set<char>& elf0, const std::set<char>& elf1, const std::set<char>& elf2) {
    for (auto e0 : elf0) {
        if (elf1.find(e0) != elf1.end() && elf2.find(e0) != elf2.end()) {
            return e0;
        }
    }
    return ' ';
}

int main()
{
    std::string file_name = "day3.txt";
    std::cout << "Advent of Code 2022\nDay 3 -  Part 1." << std::endl;
    auto lines = read_input_file(file_name.c_str());
    std::set<char> seen;
    std::string left, right;
    int n = 0, total = 0;
    for (auto line : lines) {
        n = line.size() / 2;
        seen.clear();
        for (auto i = 0; i < n; i++) {
            seen.insert(line[i]);
        }
        for (auto i = n; i < line.size(); i++) {
            if (seen.find(line[i]) != seen.end()) {
                total += priority_score(line[i]);
                break;
            }
        }
    }
    std::cout << "Total score = " << total << std::endl;

    std::cout << "Part 2." << std::endl;
    std::string line = "";
    std::set<char> elf0, elf1, elf2;
    total = 0;
    for (int i = 0; i < lines.size(); i += 3) {
        inventory(lines[i], elf0);
        inventory(lines[i + 1], elf1);
        inventory(lines[i + 2], elf2);
        total += priority_score(common_item(elf0, elf1, elf2));
    }
    std::cout << "Total score = " << total << std::endl;

}
