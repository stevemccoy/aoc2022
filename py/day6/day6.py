import fileinput

def all_different(buffer):
    return len(buffer) == len(set(buffer))

print("Advent of Code 2022, Day 6")
buffer = []
count = 0
for line in fileinput.input():
    for ch in line:
        buffer.append(ch)
        count += 1
        if len(buffer) > 14:
            del buffer[0]
        if len(buffer) == 14:
            if all_different(buffer):
                print("Found window after ", count, " characters.")
                break

print("All done.")
