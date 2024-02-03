from io import TextIOWrapper
import math
from os import path
import os
import re

SRC_DIR = "./src"
OUT = "./amalgamate/caught.h"

ENTRY_POINT_C = path.join(SRC_DIR, "main.c")
ENTRY_POINT_H = path.join(SRC_DIR, "main.h")

with open("LICENSE.txt", "r") as file:
    LICENSE = "".join(file.readlines()[:-2])

HEADER = f"""/*
{LICENSE}
================================================================================

The following file is a generated combination of all source files into one.

See https://github.com/Timothy-Gonzalez/caught for the full open source.

================================================================================
*/



"""

DIVIDER = "=" * (80 - 6)
def divider_insert(out_file, s):
    assert(len(s) < len(DIVIDER) - 1)

    left = len(DIVIDER) - len(s)
    each_side = left / 2

    out_file.write(f"\n/* {DIVIDER[0:math.floor(each_side) + 1]}{s}{DIVIDER[-math.ceil(each_side):]} */\n\n")

include_matcher = re.compile(r'\s*#include "(.*)".*')

defined_includes: set[str] = set()

def amalgamate(out_file: TextIOWrapper, filename: str):
    if not filename or not path.exists(filename) or filename in defined_includes:
        return
    
    defined_includes.add(filename)

    divider_insert(out_file, f'> {filename} <')
    
    with open(filename, 'r') as file:
        for line in file.readlines():
            match = include_matcher.match(line)

            if not match:
                out_file.write(line)
                continue

            rel_header = match.group(1)
            header = path.join(path.dirname(filename), rel_header)

            amalgamate(out_file, header)

    divider_insert(out_file, f'< {filename} >')

os.makedirs(path.dirname(OUT), exist_ok=True)

print(f"Amalgamate to: {OUT}")

with open(OUT, "w") as file:
    file.write(HEADER)
    amalgamate(file, ENTRY_POINT_H)
    amalgamate(file, ENTRY_POINT_C)
