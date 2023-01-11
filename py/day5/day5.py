import fileinput
import re

def read_input(filename):
    start = {}
    moves = []
    with fileinput.FileInput(files = (filename), mode = 'r') as input:
        for raw_line in input:
            line = raw_line.strip('\n')
            # Read starting state picture.
            if '[' in line:
                # Set up initial state dictionary if not done already.
                if not start:
                    n = 1 + (len(line) - 1) // 4
                    for i in range(n):
                        start[i+1] = []
                # Add line details to dictionary.
                i = 0
                for c in range(1, len(line), 4):
                    i += 1
                    ch = line[c]
                    if ch.isalpha():
                        start[i].append(ch)

            # Read instructions.
            if 'move' in line:
                m = re.match(r'move (?P<qty>\d+) from (?P<src>\d+) to (?P<dest>\d+)', line)
                if m:
                    d = m.groupdict()
                    moves.append((int(d['qty']), int(d['src']), int(d['dest'])))

    return start, moves

def do_move(state, move, isPart2):
    qty, src, dest = move
    if isPart2:
        if len(state[src]) < qty:
            return False
        items = state[src][:qty]
        for i in range(qty):
            state[src].pop(0)
        state[dest] = items + state[dest]
    else:
        for i in range(qty):
            if len(state[src]) < 1:
                return False
            item = state[src].pop(0)
            state[dest].insert(0, item)
    return True

def top_line(state):
    return "".join([s[0] for s in state.values()])

def part1(filename):
    state, moves = read_input(filename)
    for m in moves:
        if not do_move(state, m, False):
            print('Error when executing move ', m)
            return False
    return top_line(state)

def part2(filename):
    state, moves = read_input(filename)
    for m in moves:
        if not do_move(state, m, True):
            print('Error when executing move ', m)
            return False
    return top_line(state)

print("Advent of Code 2022 - Day 5\nPart 1.")
result = part1('day5.txt')
print("Final state top line = ", result)

print("Part 2.")
result = part2('day5.txt')
print("Final state top line = ", result)
