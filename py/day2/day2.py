import fileinput
import re

def score_round(them, us):
    if us == 'X':
        return 1 + (3 if them == 'A' else (0 if them == 'B' else 6))
    elif us == 'Y':
        return 2 + (6 if them == 'A' else (3 if them == 'B' else 0))
    elif us == 'Z':
        return 3 + (0 if them == 'A' else (6 if them == 'B' else 3))
    else:
        print("Error in score_round: Unexpected us move: ", us)
        return -1

def choose_play(them, strategy):
    if strategy == 'X':
        return ('Z' if them == 'A' else ('X' if them == 'B' else 'Y'))
    elif strategy == 'Y':
        return ('X' if them == 'A' else ('Y' if them == 'B' else 'Z'))
    elif strategy == 'Z':
        return ('Y' if them == 'A' else ('Z' if them == 'B' else 'X'))
    else:
        print("Error in choose_play: Unexpected strategy: ", strategy)
        return ' '

def part1(filename):
    total = 0
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            if line != '':
                words = re.split(r'\s', line)
                them, us = words[0], words[1]
                score = score_round(them, us)
                if score < 0:
                    print("Input error:", line)
                    return None
                total += score
    return total

def part1(filename):
    total = 0
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            if line != '':
                words = re.split(r'\s', line)
                them, us = words[0], words[1]
                score = score_round(them, us)
                if score < 0:
                    print("Input error:", line)
                    return None
                total += score
    return total

def part2(filename):
    total = 0
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            if line != '':
                words = re.split(r'\s', line)
                them, need = words[0], words[1]
                us = choose_play(them, need)
                score = score_round(them, us)
                if score < 0:
                    print("Input error:", line)
                    return None
                total += score
    return total

print("Advent of Code 2022 - Day 2\nPart 1.")
total_score = part1('day2.txt')
print("Total score = ", total_score)

print("Part 2.")
total_score = part2('day2.txt')
print("Total score = ", total_score)
