import fileinput
from glob import glob
from xmlrpc.client import MAXINT
import numpy as np
import re


# Globals.
glob_goal = (0, 0)
glob_elevation = np.array([])
glob_costs = np.array([])


def read_input_lines(filename):
    lines = []
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            lines.append(line)
    return lines

def prepare_input(lines):
    nr = len(lines)
    nc = len(lines[0])
    elevation = np.zeros((nr, nc), dtype='i')
    cost = np.zeros_like(elevation)
    cost[:] = MAXINT
    for r in range(nr):
        line = lines[r]
        for c in range(nc):
            ch = line[c]
            elevation[r, c] = (0 if ch == 'S' else (27 if ch == 'E' else ord(ch) - 96))
    return (elevation, cost)

def get_start_end(elevation):
    start = np.where(elevation == 0)
    end = np.where(elevation == 27)
    return ((int(start[0]), int(start[1])), (int(end[0]), int(end[1])))

# Set the global grids.
def setGlobals(elevation, costs, goal):
    global glob_elevation, glob_costs, glob_goal
    glob_elevation = elevation.copy()
    glob_costs = costs.copy()
    glob_goal = goal

def isGoalState(s):
    global glob_goal
    return (s[0] == glob_goal[0] and s[1] == glob_goal[1])

def set_cost(r, c, v):
    glob_costs[r, c] = v

def neighbours(s):
    result = []
    (r, c) = s[:2]
    v = glob_costs[r, c] + 1
    h = glob_elevation[r, c]
    if c > 0:
        d = glob_elevation[r, c - 1] - h
        if d < 2:
            result.append((r, c - 1, v))
    if c < glob_elevation.shape[1]:
        d = glob_elevation[r, c + 1] - h
        if d < 2:
            result.append((r, c + 1, v))
    if r > 0:
        d = glob_elevation[r - 1, c] - h
        if d < 2:
            result.append((r - 1, c, v))
    if r < glob_elevation.shape[0]:
        d = glob_elevation[r + 1, c] - h
        if d < 2:
            result.append((r + 1, c, v))
    return result

def push_open_list(item, open_list):
    (r, c, v) = item
    n = len(open_list)
    if n == 0:
        open_list.append(item)
    else:
        for i in range(n):
            if open_list[i][2] >= v:
                open_list.insert(i, item)
                break

def pop_open_list(open_list):
    if len(open_list) == 0:
        return None
    else:
        return open_list.pop(0)

def dijkstra_search(unvisited):
    while len(unvisited) > 0:
        s = pop_open_list(unvisited)
        if isGoalState(s):
            return s[2]
        nlist = neighbours(s)
        for n in nlist:
            if n[2] < glob_costs[n[0], n[1]]:
                set_cost(n[0], n[1], n[2])
                push_open_list(n, unvisited)
    return None


def part1(filename):
    lines = read_input_lines(filename)
    (elevation, cost) = prepare_input(lines)
    (start, end) = get_start_end(elevation)
    print('Start at ', start, ', End at ', end )
    setGlobals(elevation, cost, end)
    set_cost(start[0], start[1], 0)
    open_list = [(start[0], start[1], 0)]
    steps = dijkstra_search(open_list)
    if steps:
        print('Path found with ', steps, ' steps.')
    else:
        print('No path found.')

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

print("Advent of Code 2022 - Day 12\nPart 1.")
total_score = part1('test12.txt')

print("Part 2.")
#total_score = part2('day3.txt')
print("Total score = ", total_score)

