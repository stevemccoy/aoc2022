#
# Day 9 - Rope Bridge
# 

import fileinput
from glob import glob
from xmlrpc.client import MAXINT
import numpy as np
from functools import reduce
import re

class Rope:
    def __init__(self, num_knots):
        start = (0, 0)
        self.num_knots = num_knots
        self.knots = {}
        self.tail_history = { start : 1 }
        for i in range(num_knots):
            self.knots[i] = start

    def follow_knot(self, idx, hx, hy):
        if idx < self.num_knots:
            x, y = self.knots[idx]
            dx = hx - x
            dy = hy - y
            if abs(dx) > 1 or abs(dy) > 1:
                if dx == 0:
                    ex,ey = 0,(-1 if dy < 0 else 1)
                elif dy == 0:
                    ex,ey = (-1 if dx < 0 else 1),0
                else:
                    ex,ey = (-1 if dx < 0 else 1),(-1 if dy < 0 else 1)
                self.move_knot(idx, ex, ey)

    def move_knot(self, idx, dx, dy):
        if idx >= 0 and idx < self.num_knots:
            x, y = self.knots[idx]
            x += dx
            y += dy
            self.knots[idx] = (x, y)
            if idx == self.num_knots - 1:
                if (x, y) not in self.tail_history:
                    self.tail_history[(x,y)] = 1
                else:
                    self.tail_history[(x, y)] += 1
            self.follow_knot(idx + 1, x, y)

    def repeat_step_head(self, delta, steps):
        dx,dy = delta
        for i in range(steps):
            self.move_knot(0, dx, dy)

    def move_head(self, heading, distance):
        if heading == 'R':
            self.repeat_step_head((1,0), distance)
        elif heading == 'L':
            self.repeat_step_head((-1,0), distance)
        elif heading == 'U':
            self.repeat_step_head((0,1), distance)
        elif heading == 'D':
            self.repeat_step_head((0,-1), distance)
        else:
            print('Error in move_head: (', heading, ', ', distance, ')')
            return False

    def do_instruction(self, instruction):
        m = re.match(r'(?P<hdg>.) (?P<dist>\d+)', instruction)
        if m:
            d = m.groupdict()
            heading = d['hdg']
            distance = int(d['dist'])
            self.move_head(heading, distance)


def read_input_lines(filename):
    lines = []
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            lines.append(line)
    return lines

def part1(filename):
    lines = read_input_lines(filename)
    rope = Rope(2)
    for line in lines:
        rope.do_instruction(line)
    return len(rope.tail_history)

def part2(filename):
    lines = read_input_lines(filename)
    rope = Rope(10)
    for line in lines:
        rope.do_instruction(line)
    return len(rope.tail_history)
   

print('Advent of Code 2022 - Day 9.')
print('Part 1.')
count = part1('day9.txt')
print('Done following instructions. Tail visited ', count, ' places at least once.')

print('Part 2.')
count = part2('day9.txt')
print('Done following instructions. Tail visited ', count, ' places at least once.')
