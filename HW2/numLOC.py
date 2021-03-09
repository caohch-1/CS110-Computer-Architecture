import os

with open("D:\Project\CLionProjects\CS110\HW2\doubll2d.c", "r") as f:
    lines = f.readlines()
    lines = [line.strip().replace(" ","") for line in lines]
    lines = [line for line in lines if ((not line == "}") and (not line == "") )]
    print(lines)
    print(len(lines))
