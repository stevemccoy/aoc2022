#include <cstdio>

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include<fstream>
#include<string>
#include<strstream>

const int LINE_BUFFER_SIZE = 80;

typedef int64_t WorryType;

enum class Operation {
    None, Add, Multiply, Square
};

class Monkey
{
public:
    int Id;
    std::queue<WorryType> Items;
    Operation Op;
    int Arg;
    int TestDivisor;
    int IfTrue, IfFalse;
    int64_t InspectCount;

    Monkey() {
        Id = IfTrue = IfFalse = Arg = 0;
        InspectCount = 0L;
        Op = Operation::None;
        TestDivisor = 1;
    }
};

std::map<int, Monkey> monkeys;

// Use this to manage the worry levels for part 2.
WorryType modulo_base = 0;

bool expect_string(FILE* fp, const char* pattern) {
    // Skip initial space in the pattern.
    const char *pcp = &pattern[0];
    while (isspace(*pcp)) { pcp++; }
    // Skip initial space in file input.
    char fc = fgetc(fp);
    while (isspace(fc)) {
        fc = fgetc(fp);
        if (feof(fp)) {
            break;
        }
    }
    // No pattern - trivial succeed.
    if (*pcp == '\0') {
        return true;
    }
    // Look for matching chars until pattern is consumed.
    while (*pcp == fc) {
        pcp++;
        if (*pcp == '\0') {
            break;
        }
        fc = fgetc(fp);
        if (feof(fp)) {
            break;
        }
    }
    return (*pcp == '\0');
}

bool process_file(const char* file_name, std::map<int, Monkey>& monkeys) {
    Monkey m;
    char buffer[LINE_BUFFER_SIZE], tmp[LINE_BUFFER_SIZE];
    FILE* fp = nullptr;
    int id = 0, item = 0, arg = 0;
    modulo_base = 1;

    if (fopen_s(&fp, file_name, "r") == 0) {
        while (!feof(fp)) {

            fscanf_s(fp, " Monkey %d:", &id);
            m.Id = id;

            while (!m.Items.empty()) { m.Items.pop(); }
            expect_string(fp, "Starting items:");
            if (fscanf_s(fp, "%d", &item) > 0) {
                m.Items.push(item);
                while (fscanf_s(fp, ",%d", &item) > 0) {
                    m.Items.push(item);
                }
            }

            expect_string(fp, "Operation: new = old ");
            fgets(buffer, LINE_BUFFER_SIZE - 2, fp);
            if (sscanf_s(buffer, "+ %d", &arg) > 0) {
                m.Op = Operation::Add;
                m.Arg = arg;
            }
            else if (sscanf_s(buffer, "* %s", tmp, (unsigned)_countof(tmp)) > 0) {
                if (strncmp(tmp, "old", 3) == 0) {
                    m.Op = Operation::Square;
                    m.Arg = 0;
                }
                else {
                    m.Op = Operation::Multiply;
                    m.Arg = atoi(tmp);
                }
            }

            fscanf_s(fp, " Test: divisible by %d", &arg);
            m.TestDivisor = arg;
            modulo_base *= m.TestDivisor;

            fscanf_s(fp, " If true: throw to monkey %d", &arg);
            m.IfTrue = arg;
            fscanf_s(fp, " If false: throw to monkey %d", &arg);
            m.IfFalse = arg;

            monkeys[m.Id] = m;

            // Skip over white space to next entry.
            char ch = ' ';
            while (isspace(ch)) {
                ch = fgetc(fp);
            }
            ungetc(ch, fp);
        }

        fclose(fp);
    }
    else {
        printf("Unable to open file: %s\n", file_name);
    }
    return true;
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

void process_monkey(int id, bool isPart2) {
    Monkey& m = monkeys[id];
    WorryType worry = 0;
    while (m.Items.size() > 0) {
        // Inspect an item.
        worry = m.Items.front();
        if (isPart2) {
            worry = worry % modulo_base;
        }
        m.Items.pop();
        m.InspectCount++;
        switch (m.Op)
        {
        case Operation::Add:
            worry += m.Arg;
            break;
        case Operation::Multiply:
            worry *= m.Arg;
            break;
        case Operation::Square:
            worry = worry * worry;
            break;
        }
        // Get bored with item.
        if (!isPart2) {
            worry = worry / 3;
        }
        // Test and dispose of item.
        if (worry % m.TestDivisor == 0) {
            monkeys[m.IfTrue].Items.push(worry);
        }
        else {
            monkeys[m.IfFalse].Items.push(worry);
        }
    }
}

void display_items(int id) {
    Monkey& m = monkeys[id];
    printf("Monkey %d: ", m.Id);
    bool first = true;
    for (auto i : m.Items._Get_container()) {
        if (!first) {
            printf(", ");
        }
        first = false;
        printf("%lld", i);
    }
    printf("\n");
}

void display_monkeys() {
    for (auto p : monkeys) {
        display_items(p.first);
    }
    printf("\n");
}

void play_one_round(bool isPart2) {
    for (auto p : monkeys) {
        process_monkey(p.first, isPart2);
    }
}

void play_n_rounds(int n, bool isPart2) {
    for (int i = 0; i < n; i++) {
        play_one_round(isPart2);
    }
    display_monkeys();
}

void display_inspect_counts() {
    std::vector<int64_t> counts;
    for (auto p : monkeys) {
        printf("Monkey %d inspected items %lld times.\n", p.first, p.second.InspectCount);
        counts.push_back(p.second.InspectCount);
    }
    std::sort(counts.begin(), counts.end(), std::greater<int64_t>());
    int64_t monkey_business = counts[0] * counts[1];
    printf("Level of monkey business = %lld\n", monkey_business);
}

void play_n_rounds_by_stages(const std::vector<int>& rounds, bool isPart2) {
    int r = 0;
    for (auto n : rounds) {
        while (r < n) {
            play_one_round(isPart2);
            r++;
        }
        std::cout << "Rounds: " << r << std::endl;
        display_monkeys();
        display_inspect_counts();
    }
}

int main()
{
    std::cout << "Advent of Code 2022\nDay 11 -  Part 1." << std::endl;
    process_file("day11.txt", monkeys);
    play_n_rounds(20, false);
    display_inspect_counts();

    std::cout << "Part 2." << std::endl;
    monkeys.clear();
    process_file("day11.txt", monkeys);
    play_n_rounds_by_stages({ 1, 20, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 }, true);
 }
