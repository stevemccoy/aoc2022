#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include<fstream>
#include<string>
#include<algorithm>

char* grid = nullptr;
int* costs = nullptr;
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
    grid[offset(col, row)] = value;
}

char get(int col, int row) {
    return grid[offset(col, row)];
}

int get_cost(int col, int row) {
    return costs[offset(col, row)];
}

void set_cost(int col, int row, int value) {
    costs[offset(col, row)] = value;
}

/*
bool visible(int col, int row) {
    return (costs[offset(col, row)] == 't');
}

void visible(int col, int row, bool value) {
    costs[offset(col, row)] = (value ? 't' : 'f');
}
*/

void setup(const std::vector<std::string>& lines) {
    num_rows = lines.size();
    num_cols = lines[0].size();
    if (grid != nullptr) {
        delete[] grid;
        delete[] costs;
        grid = nullptr;
        costs = nullptr;
    }

    int num_chars = num_cols * num_rows * sizeof(char);
    grid = new char[num_chars];
    costs = new int[num_chars];
    memset(grid, 0, num_chars);

    for (int i = 0; i < num_chars; i++) {
        costs[i] = INT32_MAX;
    }

    int r = 0, c = 0;
    for (auto &line : lines) {
        c = 0;
        for (auto ch : line) {
            if (isupper(ch)) {
                // Set start and end nodes (S/E).
                set(c++, r, (ch == 'S' ? 'a' - 1 : 'z' + 1));
            }
            else {
                set(c++, r, ch);
            }
        }
        r++;
    }

}

typedef struct State {
    int column;
    int row;
    int cost;

    State() : column(0), row(0), cost(INT32_MAX) {}
    State(int c, int r, int v) : column(c), row(r), cost(v) {}
};

struct StateCompare {
    bool operator()(const State& s1, const State& s2) const /* noexcept */
    {
        return (s1.cost < s2.cost);
    }
};

typedef std::multiset<State, StateCompare> OpenList;

bool isGoalState(const State& s) {
    return (get(s.column, s.row) == ('z' + 1));
}

void populate_neighbours(const State& from, std::vector<State>& neighbours) {
    neighbours.clear();
    int c = from.column;
    int r = from.row;
    int v = from.cost;
    int d = 0;
    int h = get(c, r);
    State s;
    if (c > 0) {
        d = get(c - 1, r) - h;
        if (d >= 0 && d < 2) {
            s.column = c - 1;
            s.row = r;
            s.cost = v + 1;
            neighbours.push_back(s);
        }
    }
    if (c < num_cols) {
        d = get(c + 1, r) - h;
        if (d >= 0 && d < 2) {
            s.column = c + 1;
            s.row = r;
            s.cost = v + 1;
            neighbours.push_back(s);
        }
    }
    if (r > 0) {
        d = get(c, r - 1) - h;
        if (d >= 0 && d < 2) {
            s.column = c;
            s.row = r - 1;
            s.cost = v + 1;
            neighbours.push_back(s);
        }
    }
    if (r < num_rows) {
        d = get(c, r + 1) - h;
        if (d >= 0 && d < 2) {
            s.column = c;
            s.row = r + 1;
            s.cost = v + 1;
            neighbours.push_back(s);
        }
    }
}

int dijkstra_search(OpenList& unvisited) {
    State s;
    std::vector<State> neighbours;
    while (!unvisited.empty()) {
        // Pop minimum cost node off open list.
        auto bp = unvisited.begin();
        s = *bp;
        unvisited.erase(bp);
        // Stop if this is a goal node.
        if (isGoalState(s)) {
            return s.cost;
        }
        // Consider neighbours.
        populate_neighbours(s, neighbours);
        for (auto &n : neighbours) {
            if (n.cost < get_cost(n.column, n.row)) {
                set_cost(n.column, n.row, n.cost);
                unvisited.insert(n);
            }
        }
    }
    // Fall-through means no path found.
    return -1;
}

void setup_search(OpenList& unvisited) {
    unvisited.clear();
    // Find the starting node.
    State s;
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            if (get(c, r) == 'a' - 1) {
                (s.column, s.row, s.cost) = (c, r, 0);
                unvisited.insert(s);
                return;
            }
        }
    }
}

/*
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
*/

int main()
{
    OpenList unvisited;
    std::cout << "Advent of Code 2022\nDay 12 -  Part 1." << std::endl;
    auto lines = read_input_file("day12.txt");
    setup(lines);
    setup_search(unvisited);
    int cost = dijkstra_search(unvisited);
    std::cout << "Minimal path cost is " << cost << std::endl;
}
