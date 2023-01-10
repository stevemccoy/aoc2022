import fileinput

print("Advent of Code 2022, Day 7")

path = '/'
files = {}
dirs = {}

class Tree:
    
    total_size = 0
    items = []
    

for line in fileinput.input():
    if line[0] == '$':
        if "$ cd " in line:
            s = line[5:]
            if s[0] == '/':
                path = s
            else:
                path = path + '/' + s
            if dirs.
        elif "$ ls" in line:
            pass
    else:
        space = line.find(' ')
        fsize = line[0:space]
        fname = line[space+1:]
        
        pass
    
