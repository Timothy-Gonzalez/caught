from io import TextIOWrapper
import math
from os import path
import os
import re

SRC_DIR = "./src"
OUT_H = "./amalgamate/caught.h"
OUT_C = "./amalgamate/caught.c"

COLLAPSE_C_INCLUDES = '#include "caught.h"\n'

ENTRY_POINT_C = path.join(SRC_DIR, "main.c")
ENTRY_POINT_H = path.join(SRC_DIR, "lib.h")

with open("LICENSE.txt", "r") as h:
    LICENSE = "".join(h.readlines()[:-2])

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

def find_files(directory, extension):
    matched_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(extension):
                matched_files.append(os.path.join(root, file))
    return matched_files

def amalgamate(out_file: TextIOWrapper, filename: str, c: bool = False):
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

            if c:
                if COLLAPSE_C_INCLUDES not in defined_includes:
                    defined_includes.add(COLLAPSE_C_INCLUDES)
                    out_file.write(COLLAPSE_C_INCLUDES)
                continue

            rel_header = match.group(1)


            header = path.join(path.dirname(filename), rel_header)

            amalgamate(out_file, header)

    divider_insert(out_file, f'< {filename} >')

os.makedirs(path.dirname(OUT_H), exist_ok=True)

print(f"Amalgamate to: {OUT_H} and {OUT_C}")

with open(OUT_H, "w") as h:
    h.write(HEADER)
    amalgamate(h, ENTRY_POINT_H)

    # grab any missed h files
    for file in find_files(SRC_DIR, ".h"):
        amalgamate(h, file)
    

with open(OUT_C, "w") as c:
    c.write(HEADER)

    # grab all c files (order doesn't matter now)
    for file in find_files(SRC_DIR, ".c"):
        amalgamate(c, file, True)
    
