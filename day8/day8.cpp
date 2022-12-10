#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include<fstream>
#include<string>
#include<algorithm>

char* wood = nullptr;
char* vis_grid = nullptr;
int num_cols = 0, num_rows = 0;

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

int offset(int col, int row) {
    return row * num_cols + col;
}

void set(int col, int row, char value) {
    wood[offset(col, row)] = value;
}

char get(int col, int row) {
    return wood[offset(col, row)];
}

bool visible(int col, int row) {
    return (vis_grid[offset(col, row)] == 't');
}

void visible(int col, int row, bool value) {
    vis_grid[offset(col, row)] = (value ? 't' : 'f');
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

void sweep_east() {
    int g, h;
    for (int r = 0; r < num_rows; r++) {
        g = -1;
        for (int c = 0; c < num_cols; c++) {
            h = get(c, r);
            if (h > g) {
                visible(c, r, true);
                g = h;
            }
        }
    }
}

void sweep_west() {
    int g, h;
    for (int r = 0; r < num_rows; r++) {
        g = -1;
        for (int c = num_cols - 1; c >= 0; c--) {
            h = get(c, r);
            if (h > g) {
                visible(c, r, true);
                g = h;
            }
        }
    }
}

void sweep_south() {
    int g, h;
    for (int c = 0; c < num_cols; c++) {
        g = -1;
        for (int r = 0; r < num_rows; r++) {
            h = get(c, r);
            if (h > g) {
                visible(c, r, true);
                g = h;
            }
        }
    }
}

void sweep_north() {
    int g, h;
    for (int c = 0; c < num_cols; c++) {
        g = -1;
        for (int r = num_rows - 1; r >= 0; r--) {
            h = get(c, r);
            if (h > g) {
                visible(c, r, true);
                g = h;
            }
        }
    }
}

void do_sweeps()
{
    sweep_east();
    sweep_north();
    sweep_west();
    sweep_south();
}

int count_visibles()
{
    int count = 0;
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            if (visible(c, r)) {
                count++;
            }
        }
    }
    return count;
}

void display()
{
    std::cout << std::endl;
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            std::cout << (char)(get(c, r) + '0') << (visible(c, r) ? '+' : '.');
        }
        std::cout << std::endl;
    }
}

int viewing_distance(int col, int row, int direction) {
    int count = 0;
    int h = get(col, row);
    switch (direction) {
    case 0:
        // North
        for (int i = row - 1; i >= 0; i--) {
            count++;
            if (get(col, i) >= h) {
                break;
            }
        }
        break;
    case 1:
        // East
        for (int i = col + 1; i < num_cols; i++) {
            count++;
            if (get(i, row) >= h) {
                break;
            }
        }
        break;
    case 2:
        // South
        for (int i = row + 1; i < num_rows; i++) {
            count++;
            if (get(col, i) >= h) {
                break;
            }
        }
        break;
    case 3:
        // West
        for (int i = col - 1; i >= 0; i--) {
            count++;
            if (get(i, row) >= h) {
                break;
            }
        }
        break;
    }
    return count;
}

int scenic_score(int col, int row) {
    int score = 1;
    for (int direction = 0; direction < 4; direction++) {
        score *= viewing_distance(col, row, direction);
    }
    return score;
}

int best_scenic_score() {
    int best = 0, score = 0;
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            score = scenic_score(col, row);
            if (score > best) {
                best = score;
            }
        }
    }
    return best;
}

int main()
{
    std::cout << "Advent of Code 2022\nDay 8 -  Part 1." << std::endl;
    auto lines = read_input_file("day8.txt");
    setup_wood(lines);
    display();
    do_sweeps();
    display();
    int count = count_visibles();
    std::cout << "Result: " << count << " visible sites." << std::endl;

    std::cout << "Part 2." << std::endl;
    int score = best_scenic_score();
    std::cout << "Best scenic score in wood = " << score << std::endl;
}
