#
# Day 15 - Beacon Exclusion Zone
#

import fileinput
from lib2to3.pgen2 import token
import re

def read_input_lines(filename):
	lines = []
	pattern = r'Sensor at x=(?P<sx>.+), y=(?P<sy>.+): closest beacon is at x=(?P<bx>.+), y=(?P<by>.+)'
	with fileinput.FileInput(files = (filename), mode = 'r') as input:
		for raw_line in input:
			line = raw_line.strip(' \n')
			m = re.match(pattern, line)
			if m:
				d = m.groupdict()
				lines.append((int(d['sx']), int(d['sy']), int(d['bx']), int(d['by'])))
	return lines

def manhattan_distance(a, b):
	fx,fy = a
	tx,ty = b
	return abs(tx - fx) + abs(ty - fy)

def add_to_zone(zone, x, y, bx, by):
	if y != by or x != bx:
		zone[x] = 1

def exclusion_zone(lines, y):
	zone = {}
	for line in lines:
		sx,sy = line[0],line[1]
		bx,by = line[2],line[3]
		d = manhattan_distance((sx,sy), (bx,by))
		dy = d - abs(sy - y)
		if dy > 0:
			for i in range(dy + 1):
				add_to_zone(zone, sx - i, y, bx, by)
				add_to_zone(zone, sx + i, y, bx, by)
	return zone

def tuning_frequency(x, y):
	return 4000000 * x + y

def overlap(f1, t1, f2, t2):
    return (f2 >= f1 and t1 >= f2) or (f1 >= f2 and f1 <= t2)

def interval_union(i1, i2):
	return (min(i1[0], i2[0]), max(i1[1], i2[1]))

def exclusion_intervals(lines, y):
	zone = []
	# Populate exclusion zone intervals.
	for line in lines:
		sx,sy = line[0],line[1]
		bx,by = line[2],line[3]
		d = manhattan_distance((sx,sy), (bx,by))
		dy = d - abs(sy - y)
		if dy >= 0:
			zone.append((sx - dy, sx + dy))
	# Merge intervals where possible.
	i = 0
	while i < len(zone):
		i1 = zone[i]
		change = True
		while change:
			change = False
			j = i + 1
			while j < len(zone):
				i2 = zone[j]
				if overlap(i1[0], i1[1], i2[0], i2[1]):
					zone[i] = i1 = interval_union(i1, i2)
					del zone[j]
					change = True
				else:
					j += 1
		i += 1
	return sorted(zone, key=lambda e: e[0])

def find_distress_beacon(lines, limit):
	for y in range(limit + 1):
		intervals = exclusion_intervals(lines, y)
		if len(intervals) > 1:
			x = (intervals[1][0] + intervals[0][1]) // 2
			return (x,y)
	return None

def part1(filename, row):
	report = read_input_lines(filename)
	zone = exclusion_zone(report, row)
	return len(zone)

def part2(filename, limit):
	report = read_input_lines(filename)
	x,y = find_distress_beacon(report, limit)
	print('Distress beacon at ', (x,y))
	f = tuning_frequency(x, y)
	return f

print('Advent of Code 2022\nDay 15: Beacon Exclusion Zone')
print('Part 1.')
row = 2000000
count = part1('day15.txt', row)
print(f"Number of excluded locations in row {row} = {count}")

print('Part 2.')
limit = 4000000
frequency = part2('day15.txt', limit)
print(f"Tuning frequency of distress beacon = {frequency}")
