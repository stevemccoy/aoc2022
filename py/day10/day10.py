#
# Day 10 - Cathode Ray Tube.
# 

import fileinput
from functools import reduce

class Cpu:
    def __init__(self):
        self.X = 1
        self.clock = 0
        self.history = [(0,1)]

    def addx(self, amount):
        self.clock += 2
        self.X += amount
        self.history.append((self.clock, self.X))

    def noop(self):
        self.clock += 1

    def signal(self):
        return self.clock * self.X

    def lookup_x(self, cycle):
        _,x = self.history[0]
        for t,xt in self.history:
            if t >= cycle:
                return x
            x = xt
        return x


def execute_input_file(filename):
    cpu = Cpu()
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip(' \n')
            if line.startswith('addx'):
                amount = int(line[5:])
                cpu.addx(amount)
            elif line.startswith('noop'):
                cpu.noop()
    return cpu

def signal_strength_at_timepoints(cpu, timepoints):
    result = []
    for t in timepoints:
        result.append(cpu.lookup_x(t) * t)
    return result

def render_image(filename):
    image = []
    cpu = execute_input_file(filename)
    for offset in [0,40,80,120,160,200]:
        line = []
        for x in range(40):
            d = abs(cpu.lookup_x(offset + x + 1) - x)
            pxl = '#' if d < 2 else '.'
            line.append(pxl)
        image.append("".join(line))
    return image

def part1(filename):
    timepoints = [20,60,100,140,180,220]
    print('Timepoints = ', timepoints)
    cpu = execute_input_file(filename)
    result = signal_strength_at_timepoints(cpu, timepoints)
    print('Signal strengths: ', result)
    sum = reduce(lambda x, y: x + y, result)
    return sum

print('Advent of Code 2022 - Day 10.')
print('Part 1.')
count = part1('day10.txt')
print('Done following instructions. Sum of signal strengths = ', count)

print('Part 2.')
image = render_image('day10.txt')
for line in image:
    print(line)
print('\nAll Done.')
