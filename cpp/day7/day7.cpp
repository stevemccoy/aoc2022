#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
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

void process_instruction(std::string instruction, std::string& path, std::map<std::string, int>& result) {
    char buffer[80] = "";
    int file_size = 0;
    int subdir_amount = 0;
    if (sscanf_s(instruction.c_str(), "$ cd %s", buffer, (unsigned)_countof(buffer)) != 0) {
        if (buffer[0] == '/') {
            path = buffer;
        }
        else if (strncmp(buffer, "..", 2) == 0) {
            subdir_amount = result[path];
            path = path.substr(0, path.size() - 1);
            path = path.substr(0, path.find_last_of('/') + 1);
            result[path] += subdir_amount;
        }
        else {
            path += buffer;
            path += "/";
        }
        result[path] += 0;
    }
    else if (sscanf_s(instruction.c_str(), "%d %s", &file_size, buffer, (unsigned)_countof(buffer)) != 0) {
        result[path] += file_size;
    }
}

std::map<std::string, int> directory_details(const std::vector<std::string>& instructions) {
    std::map<std::string, int> result;
    std::string path = "/";
    // Process given instructions.
    for (auto i : instructions) {
        process_instruction(i, path, result);
    }
    // Get back to root directory.
    while (path != "/") {
        process_instruction("$ cd ..", path, result);
    }
    return result;
}

int sum_small_directory_sizes(const std::map<std::string, int>& dirs) {
    int total = 0;
    for (auto d : dirs) {
        if (d.second < 100000) {
            total += d.second;
        }
    }
    return total;
}

int choose_directory_to_delete(const std::map<std::string, int>& details) {
    const int filesystem_size = 70000000;
    const int freespace_needed = 30000000;
    int amount_used = details.find("/")->second;
    int freespace_now = filesystem_size - amount_used;
    int amount_to_reclaim = freespace_needed - freespace_now;
    int result = amount_used;
    for (auto d : details) {
        if ((d.second > amount_to_reclaim) && (d.second < result)) {
            result = d.second;
        }
    }
    return result;
}

int main()
{
    std::cout << "Advent of Code 2022\nDay 7 -  Part 1." << std::endl;
    std::cout << "Test:" << std::endl;
    auto lines = read_input_file("test7.txt");
    auto details = directory_details(lines);
    std::cout << "Result: " << sum_small_directory_sizes(details) << std::endl;
    std::cout << "Size of directory to delete: " << choose_directory_to_delete(details) << std::endl;

    std::cout << "For Real:" << std::endl;
    lines = read_input_file("day7.txt");
    details = directory_details(lines);
    std::cout << "Result: " << sum_small_directory_sizes(details) << std::endl;
    std::cout << "Size of directory to delete: " << choose_directory_to_delete(details) << std::endl;
}
