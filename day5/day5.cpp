// day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <iostream>
#include<fstream>
#include<string>
#include<algorithm>

std::stack<char> stacks[9];

void clear_stacks() {
    for (int i = 0; i < 9; i++) {
        while (!stacks[i].empty()) {
            stacks[i].pop();
        }
    }
}

void setup_stacks(bool isTest) {
    clear_stacks();
    if (isTest) {
        const char* stack1 = "ZN";
        const char* stack2 = "MCD";
        const char* stack3 = "P";
        for (const char* cp = stack1; *cp != '\0'; cp++) { stacks[0].push(*cp); }
        for (const char* cp = stack2; *cp != '\0'; cp++) { stacks[1].push(*cp); }
        for (const char* cp = stack3; *cp != '\0'; cp++) { stacks[2].push(*cp); }
    }
    else {
        const char* stack1 = "RPCDBG";
        const char* stack2 = "HVG";
        const char* stack3 = "NSQDJPM";
        const char* stack4 = "PSLGDCNM";
        const char* stack5 = "JBNCPFLS";
        const char* stack6 = "QBDZVGTS";
        const char* stack7 = "BZMHFTQ";
        const char* stack8 = "CMDBF";
        const char* stack9 = "FCQG";
        for (const char* cp = stack1; *cp != '\0'; cp++) { stacks[0].push(*cp); }
        for (const char* cp = stack2; *cp != '\0'; cp++) { stacks[1].push(*cp); }
        for (const char* cp = stack3; *cp != '\0'; cp++) { stacks[2].push(*cp); }
        for (const char* cp = stack4; *cp != '\0'; cp++) { stacks[3].push(*cp); }
        for (const char* cp = stack5; *cp != '\0'; cp++) { stacks[4].push(*cp); }
        for (const char* cp = stack6; *cp != '\0'; cp++) { stacks[5].push(*cp); }
        for (const char* cp = stack7; *cp != '\0'; cp++) { stacks[6].push(*cp); }
        for (const char* cp = stack8; *cp != '\0'; cp++) { stacks[7].push(*cp); }
        for (const char* cp = stack9; *cp != '\0'; cp++) { stacks[8].push(*cp); }
    }
}

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

bool check_location(int loc) {
    return ((loc > 0) && (loc <= 9));
}

bool do_instruction(int amount, int src, int dest, bool isPart2) {
    if (!check_location(src)) {
        std::cerr << "Bad source location: " << src << std::endl;
        return false;
    }
    if (!check_location(dest)) {
        std::cerr << "Bad destination location: " << dest << std::endl;
        return false;
    }
    if (amount < 0) {
        std::cerr << "Bad amount: " << amount << std::endl;
        return false;
    }
    src--;
    dest--;
    if (!isPart2) {
        while (amount--) {
            if (stacks[src].empty()) {
                std::cerr << "Source stack empty." << std::endl;
                return false;
            }
            char ch = stacks[src].top();
            stacks[src].pop();
            stacks[dest].push(ch);
        }
    }
    else {
        std::stack<char> temp;
        while (amount--) {
            char ch = stacks[src].top();
            stacks[src].pop();
            temp.push(ch);
        }
        while (!temp.empty()) {
            char ch = temp.top();
            temp.pop();
            stacks[dest].push(ch);
        }
    }
    return true;
}

void process_file(const char* file_name, bool isPart2) {
    std::vector<std::string> lines = read_input_file(file_name);
    int amount, src, dest;
    for (auto line : lines) {
        if (line.find("move") == std::string::npos) {
            continue;
        }
        sscanf_s(line.c_str(), "move %d from %d to %d", &amount, &src, &dest);
        do_instruction(amount, src, dest, isPart2);
    }
}

std::string top_string() {
    std::string result = "";
    for (int i = 0; i < 9; i++) {
        if (!stacks[i].empty()) {
            result += stacks[i].top();
        }
    }
    return result;
}

int main()
{
    std::string file_name = "day5.txt";
    std::cout << "Advent of Code 2022\nDay 5 -  Part 1." << std::endl;
    std::cout << "Test:" << std::endl;
    setup_stacks(true);
    process_file("test5.txt", false);
    std::cout << "Result: " << top_string() << std::endl;

    std::cout << "Not a Test:" << std::endl;
    setup_stacks(false);
    process_file("day5.txt", false);
    std::cout << "Result: " << top_string() << std::endl;

    std::cout << "Part 2." << std::endl;
    std::cout << "Test:" << std::endl;
    setup_stacks(true);
    process_file("test5.txt", true);
    std::cout << "Result: " << top_string() << std::endl;

    std::cout << "Not a Test:" << std::endl;
    setup_stacks(false);
    process_file("day5.txt", true);
    std::cout << "Result: " << top_string() << std::endl;
}
