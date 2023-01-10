import fileinput
import re

def extract_line(line):
    m = re.match(r'(?P<f1>\d+)-(?P<t1>\d+),(?P<f2>\d+)-(?P<t2>\d+)', line)
    if m:
        d = m.groupdict()
        return ((int(d['f1']), int(d['t1'])), (int(d['f2']), int(d['t2']))) 
    else:
        return None

def containment(f1, t1, f2, t2):
    return (f1 >= f2 and t1 <= t2) or (f2 >= f1 and t2 <= t1)

def overlap(f1, t1, f2, t2):
    return (f2 >= f1 and t1 >= f2) or (f1 >= f2 and f1 <= t2)

def solve(filename, condition):
    count = 0
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            e = extract_line(line)
            if e:
                (f1,t1) = e[0]
                (f2,t2) = e[1]
                if condition(f1, t1, f2, t2):
                    count += 1
    return count

def part1(filename):
    return solve(filename, containment)

def part2(filename):
    return solve(filename, overlap)

print("Advent of Code 2022 - Day 4\nPart 1.")
count = part1('day4.txt')
print("Count = ", count)

print("Part 2.")
count = part2('day4.txt')
print("Count = ", count)
