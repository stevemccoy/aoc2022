#
# Day 12 - Hill Climbing.
# 
# Uniform cost search for shortest path through finite grid search space.
#

import fileinput
from glob import glob
from xmlrpc.client import MAXINT
import numpy as np

# Globals.
glob_goal = (0, 0)
glob_elevation = np.array([], dtype='i')
glob_costs = np.array([], dtype='i')


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
def setGlobals(elevation, costs, start, goal):
    global glob_elevation, glob_costs, glob_goal
    glob_elevation = elevation.copy()
    glob_costs = costs.copy()
    glob_goal = goal
    glob_elevation[start[0], start[1]] = 1
    glob_elevation[goal[0], goal[1]] = 26

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
    if c < glob_elevation.shape[1] - 1:
        d = glob_elevation[r, c + 1] - h
        if d < 2:
            result.append((r, c + 1, v))
    if r > 0:
        d = glob_elevation[r - 1, c] - h
        if d < 2:
            result.append((r - 1, c, v))
    if r < glob_elevation.shape[0] - 1:
        d = glob_elevation[r + 1, c] - h
        if d < 2:
            result.append((r + 1, c, v))
    return result

def push_open_list(item, open_list):
    (r, c, v) = item
    n = len(open_list)
    for i in range(n):
        if open_list[i][2] > v:
            open_list.insert(i, item)
            return
    open_list.append(item)

def pop_open_list(open_list):
    if len(open_list) == 0:
        return None
    else:
        return open_list.pop(0)

def find_loc_open_list(item, open_list):
    r,c = item[:2]
    for loc in open_list:
        if loc[0] == r and loc[1] == c:
            return loc
    return None

def open_list_replace(open_list, item, replacement):
    open_list.remove(item)
    push_open_list(replacement, open_list)

# Return the minimum path cost from start to a goal node.
def uniform_cost_search(startNodes, goalTest):
    frontier = [i for i in startNodes]
    node = frontier[0]
    expanded = []
    while len(frontier) > 0:
        node = pop_open_list(frontier)
        if goalTest(node):
            return node
        expanded.append(node[:2])
        nlist = neighbours(node)
        for n in nlist:
            fitem = find_loc_open_list(n, frontier)
            if fitem:
                if fitem[2] > n[2]:
                    open_list_replace(frontier, fitem, n)
                    set_cost(n[0], n[1], n[2])
            elif n[:2] not in expanded:
                push_open_list(n, frontier)
                set_cost(n[0], n[1], n[2])
    return None

def part1(filename):
    print('Part 1.')
    lines = read_input_lines(filename)
    (elevation, cost) = prepare_input(lines)
    (start, end) = get_start_end(elevation)
    print('Start at ', start, ', End at ', end )
    setGlobals(elevation, cost, start, end)
    set_cost(start[0], start[1], 0)
    node = uniform_cost_search([(start[0], start[1], 0)], isGoalState)
    if node:
        print('Path found with ', node[2], ' steps.')
    else:
        print('No path found.')

def part2(filename):
    print('Part 2.')
    lines = read_input_lines(filename)
    (elevation, cost) = prepare_input(lines)
    (start, end) = get_start_end(elevation)
    print('Start at ', start, ', End at ', end )
    setGlobals(elevation, cost, start, end)
    # Start at any square with an 'a':
    starts = []
    for idx,v in np.ndenumerate(glob_elevation):
        if v == 1:
            starts.append((idx[0], idx[1], 0))
            set_cost(idx[0], idx[1], 0)
    steps = uniform_cost_search(starts, isGoalState)
    if steps:
        print('Path found with ', steps[2], ' steps.')
    else:
        print('No path found.')

print("Advent of Code 2022 - Day 12.")
part1('day12.txt')

part2('day12.txt')
