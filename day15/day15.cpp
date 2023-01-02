#include <cstdio>
#include <iostream>
#include <cmath>
#include <map>
#include<fstream>
#include<string>
#include<vector>

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

int manhattan_distance(int from_x, int from_y, int to_x, int to_y) {
    return (abs(from_x - to_x) + abs(from_y - to_y));
}

class SensorReport {
public:
    int SensorX, SensorY, BeaconX, BeaconY, Range;

    SensorReport(int sx, int sy, int bx, int by) : SensorX(sx), SensorY(sy), BeaconX(bx), BeaconY(by)
    {
        Range = manhattan_distance(sx, sy, bx, by);
    }
};

std::vector<SensorReport> process_input(const std::vector<std::string>& lines) {
    std::vector<SensorReport> reports;
    int sx, sy, bx, by;
    for (auto line : lines) {
        if (sscanf_s(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", 
            &sx, &sy, &bx, &by) > 0) 
        {
            reports.push_back(SensorReport(sx, sy, bx, by));
        }
    }
    return reports;
}

int excluded_positions_in_row(int row, const std::vector<SensorReport>& sensors) {
    std::map<int, int> exclude_count;
    for (auto s : sensors) {
        if (abs(s.SensorY - row) <= s.Range) {

        }
    }
}

int main()
{
    std::cout << "Advent of Code 2022\nDay 15 -  Part 1." << std::endl;
    std::cout << "Test:" << std::endl;
    auto lines = read_input_file("test15.txt");
    auto reports = process_input(lines);
    //    make_moves(lines, false);
//    std::cout << "Tail has been to " << trail.size() << " locations." << std::endl;

    std::cout << "Trial:" << std::endl;
    lines = read_input_file("day9.txt");

//    make_moves(lines, false);
//    std::cout << "Tail has been to " << trail.size() << " locations." << std::endl;

/*
    std::cout << "Part 2.\nTest:" << std::endl;
    lines = read_input_file("test92.txt");
    make_moves(lines, true);
    std::cout << "Tail has been to " << trail.size() << " locations." << std::endl;

    std::cout << "Trial:" << std::endl;
    lines = read_input_file("day9.txt");
    make_moves(lines, true);
    std::cout << "Tail has been to " << trail.size() << " locations." << std::endl;
*/
}
