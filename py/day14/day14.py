#
# Day 14 - Regolith Reservoir.
#

import numpy as np
import re

# Globals.

# Canvas for the game.
num_rows = num_cols = 1000
grid = np.zeros((num_rows, num_cols), dtype = np.int8)

# Read the contents of the given file.
def gulp_input(file_name):
	with open(file_name, 'r') as f:
		return list(f)

def draw_line(x1, y1, x2, y2):
#	print((x1,y1), ' to ', (x2,y2))
	count = 0
	if x1 == x2:
		for y in range(min(y1,y2), max(y1,y2)+1):
			grid[y, x1] = 2
			count += 1
	elif y1 == y2:
		for x in range(min(x1,x2), max(x1,x2)+1):
			grid[y1, x] = 2
			count += 1
	else:
		print('Error in draw_line', (x1, y1, x2, y2))
	return count

# Mark the positions of the rock formations on the grid.
def draw_rocks(lines):
	count = 0
	pattern = r'(?P<x>\d+),(?P<y>\d+)'
	for line in lines:
		d = re.match(pattern, line).groupdict()
		x1,y1 = int(d['x']),int(d['y'])
		s = line
		p = line.find(" ->")
		while p != -1:
			s = s[p+4:]
			e = s.find(" ")
			ps = s if e == -1 else s[:e] 
			d = re.match(pattern, ps).groupdict()
			x2,y2 = int(d['x']),int(d['y'])
			# Draw the line HERE.
			count += draw_line(x1, y1, x2, y2)
			x1,y1 = x2,y2
			p = s.find(" ->")
	return count

def drop_sand_grain():
	x,y = 500,0
	if grid[y, x] != 0:		# Inlet blocked.
		return False
	while y < 999:
		if grid[y + 1, x] == 0:
			y += 1
		elif grid[y + 1, x - 1] == 0:
			x += -1
			y += 1
		elif grid[y + 1, x + 1] == 0:
			x += 1
			y += 1
		else:
			grid[y, x] = 1
			return True
	# Get to here and the sand falls off into the void.
	return False

def limit_bounds(a):
	rows,cols = np.nonzero(a)
	return (min(rows), min(cols), max(rows), max(cols))

def print_grid(g, bounds = None):
	fy,fx,ty,tx = bounds if bounds else limit_bounds(g)
	print(f"From: ({fx}, {fy}), To: ({tx}, {ty})")
	for y in range(fy, ty + 1, 1):
		s = []
		for x in range(fx, tx + 1, 1):
			ch = ' ' if g[y, x] == 0 else '#' if g[y, x] == 2 else 'o'
			s.append(ch)
		print("".join(s))

def part1_for(file_name):
	global grid
	grid = np.zeros((num_rows, num_cols), dtype = np.int8)
	lines = gulp_input(file_name)
	count = draw_rocks(lines)
	print('Drew ', count, ' rock cells')
#	print_grid(grid)
	grains = 0
	while drop_sand_grain():
		grains += 1
	print('Dropped ', grains, ' sand grains.')
	drop_sand_grain()
	print_grid(grid)

def part2_for(file_name):
	global grid
	grid = np.zeros((num_rows, num_cols), dtype = np.int8)
	lines = gulp_input(file_name)
	count = draw_rocks(lines)
	fy,fx,ty,tx = limit_bounds(grid)
	ty += 2
	draw_rocks([f"0,{ty} -> {num_cols - 1},{ty}"])
	print('Drew ', count, ' rock cells')
	print_grid(grid, (fy,fx,ty,tx))

	grains = 0
	while drop_sand_grain():
		grains += 1

	print('Dropped ', grains, ' sand grains.')
	print_grid(grid, (fy,fx,ty,tx))


# Main processing.
print('Advent of Code 2022 - Day 14, Part 1.')
print('Running test...')
part1_for('test14.txt')

print('Running full set...')
part1_for('day14.txt')

print('Part 2.')
print('Running test...')
part2_for('test14.txt')

print('Running full set...')
part2_for('day14.txt')
