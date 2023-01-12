#
# Day 8 - Treetop Tree House.
# 

import fileinput
from glob import glob
from xmlrpc.client import MAXINT
import numpy as np
from functools import reduce


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
    for r in range(nr):
        line = lines[r]
        for c in range(nc):
            ch = line[c]
            elevation[r, c] = int(ch)
    return elevation

def visible_north(e, r, c):
    h = e[r, c]
    return (r == 0) or all([e[x, c] < h for x in range(r)])

def visible_east(e, r, c):
    h = e[r, c]
    nc = e.shape[1]
    return (c == nc - 1) or all([e[r, x] < h for x in range(c + 1, nc)])

def visible_south(e, r, c):
    h = e[r, c]
    nr = e.shape[0]
    return (r == nr - 1) or all([e[x, c] < h for x in range(r + 1, nr)])

def visible_west(e, r, c):
    h = e[r, c]
    return (c == 0) or all([e[r, x] < h for x in range(c)])

# On elevation map e, is cell (r, c) visible from outside the grid?
def visible(e, r, c):
    return visible_north(e, r, c) or visible_east(e, r, c) or visible_south(e, r, c) or visible_west(e, r, c)

def visible_tuple(e, item):
    r, c = item
    return visible(e, r, c)

# Find viewing distance on map e, from (r, c) in a given direction dir.
def view_distance(e, r, c, dir):
    count = 0
    h = e[r, c]
    if dir == 0:        # North
        for y in range(r - 1, -1, -1):
            count += 1
            if e[y, c] >= h:
                break
    elif dir == 1:      # East
        for x in range(c + 1, e.shape[1], 1):
            count += 1
            if e[r, x] >= h:
                break
    elif dir == 2:      # South
        for y in range(r + 1, e.shape[0], 1):
            count += 1
            if e[y, c] >= h:
                break
    elif dir == 3:      # West
        for x in range(c - 1, -1, -1):
            count += 1
            if e[r, x] >= h:
                break
    else:
        print('Error in view_distance, dir = ', dir)
    return count

def scenic_score(e, r, c):
    distances = [view_distance(e, r, c, d) for d in [0, 1, 2, 3]]
    return reduce((lambda x, y: x * y), distances, 1)

def scenic_score_tuple(e, item):
    r,c = item
    return scenic_score(e, r, c)

def part1(filename):
    lines = read_input_lines(filename)
    elevation = prepare_input(lines)
    return [visible_tuple(elevation, idx) for idx,_ in np.ndenumerate(elevation) ].count(True)

def part2(filename):
    lines = read_input_lines(filename)
    elevation = prepare_input(lines)
    scores = [scenic_score_tuple(elevation, idx) for idx,_ in np.ndenumerate(elevation) ]
    return max(scores)
   

print('Advent of Code 2022 - Day 8.')
print('Part 1.')
count = part1('day8.txt')
print('Number of visible trees = ', count)

print('Part 2.')
score = part2('day8.txt')
print('Max scenic score for trees in the grid = ', score)
