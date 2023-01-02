import numpy as np

# Globals.

# Canvas for the game.
num_rows = num_cols = 1000
grid = np.zeros((num_rows, num_cols), dtype = np.int8)


# Read the contents of the given file.
def process_input(file_name):
	with open(file_name, 'r') as f:
		return list(f)

def draw_line(x1, y1, x2, y2):
#	print((x1,y1), ' to ', (x2,y2))
	count = 0
	if x1 == x2:
		for y in range(min(y1,y2), max(y1,y2)+1):
			grid[x1,y] = 1
			count += 1
	elif y1 == y2:
		for x in range(min(x1,x2), max(x1,x2)+1):
			grid[x,y1] = 1
			count += 1
	return count

# Mark the positions of the rock formations on the grid.
def draw_rocks(lines):
	count = 0
	for line in lines:
		x1 = int(line[:line.find(",")])
		y1 = int(line[line.find(",")+1])
#		print((x1,y1))
		s = line
		p = line.find(" ->")
		while p != -1:
			s = s[p+4:]
			e = s.find(" ")
			ps = s if e == -1 else s[:e] 
			x2 = int(ps[:ps.find(",")])
			y2 = int(ps[ps.find(",")+1:])
#			print((x2,y2))
			# Draw the line HERE.
			count += draw_line(x1, y1, x2, y2)
			x1,y1 = x2,y2
			p = s.find(" ->")
	return count

def drop_sand_grain():
	x,y = 500,0
	while y < 999:
		if grid[x, y + 1] == 0:
			y += 1
		elif grid[x - 1, y + 1] == 0:
			x += -1
			y += 1
		elif grid[x + 1, y + 1] == 0:
			x += 1
			y += 1
		else:
			grid[x, y] = 1
			return True
	# Get to here and the sand falls off into the void.
	return False

def part1_for(file_name):
	global grid
	grid = np.zeros((num_rows, num_cols), dtype = np.int8)

	lines = process_input(file_name)
	count = draw_rocks(lines)
	print('Drew ', count, ' rock cells')

	grains = 0
	while drop_sand_grain():
		grains += 1

	print('Dropped ', grains, ' sand grains.')



# Main processing.
print('Advent of Code 2022 - Day 14, Part 1.')
print('Running test...')
part1_for('day14.txt')

print('Running full set...')
part1_for('day14.txt')

