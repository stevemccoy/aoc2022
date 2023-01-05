import fileinput
import re

def priority_score(ch):
    return (ord(ch) - 97 + 1) if ch.islower() else ((ord(ch) - 65 + 27) if ch.isupper() else 0)

def inventory(line):
    seen = {}
    for c in line:
        seen[c] = 1
    return seen

def common_item(elf0, elf1, elf2):
    for c in elf0:
        if c in elf1 and c in elf2:
            return c
    return ' '

def part1(filename):
    seen = {}
    total = 0
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            if line != '':
                n = len(line) // 2
                seen.clear()
                for c in line[:n]:
                    seen[c] = 1
                for c in line[n:]:
                    if c in seen:
                        total += priority_score(c)
                        break
    return total

def part2(filename):
    total = 0
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        lines = []
        for raw_line in input:
            line = raw_line.strip(' \n')
            if line != '':
                lines.append(line)
    for i in range(0, len(lines), 3):
        elf0 = inventory(lines[i])
        elf1 = inventory(lines[i + 1])
        elf2 = inventory(lines[i + 2])
        total += priority_score(common_item(elf0, elf1, elf2))
    return total

print("Advent of Code 2022 - Day 3\nPart 1.")
total_score = part1('day3.txt')
print("Total score = ", total_score)

print("Part 2.")
total_score = part2('day3.txt')
print("Total score = ", total_score)
