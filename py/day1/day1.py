import fileinput

calories = []

with fileinput.FileInput(files = ('day1.txt'), mode = 'r') as input:
    cals = 0
    for raw_line in input:
        line = raw_line.strip(' \n')
        if line == '':
            calories.append(cals)
            cals = 0
        else:
            cals += int(line)
    if cals > 0:
        calories.append(cals)

elf_totals = sorted(calories, reverse=True)
max_calories = elf_totals[0]
print("Advent of Code 2022 - Day 1\nPart 1.")
print("Max Calories carried by a single elf = ", max_calories)

print("Part 2.")
total_calories = sum(elf_totals[:3])
print("Total calories carried by 3 top elves = ", total_calories)


