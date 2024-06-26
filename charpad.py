#!/usr/bin/env python3

# Python script to convert character patterns exported from CharPad to AgonLight
# Copyright 2024 J.B. Langston
#
# Permission is hereby granted, free of charge, to any person obtaining a 
# copy of this software and associated documentation files (the "Software"), 
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.

# HOWTO: Convert C64 character definitions exported by CharPad to format useable by Agon VDP...
# 1) Download and install the following:
#    - VICE: https://vice-emu.sourceforge.io/index.html#download (GTK3 version)
#    - CharPad: https://csdb.dk/release/?id=200341
#    - Python 3: https://www.python.org/downloads/
# 2) Run C64 program in VICE; advance program until the part you are interested in
# 3) Save a snapshot image
# 4) Import the snapshot image from step 3 in CharPad
# 5) Export characters to text/asm
# 6) Pipe output from step 5 through charpad.py to convert


import fileinput

# Customization constants
INPUT_PREFIX = ".byte "     # prefix of lines to input
MAX_LINES = 32              # maximum number of lines to input
SKIP_CHARS = 1              # Number of characters to skip (comment out)
START_CHAR = 128            # Starting character number
INDENT = "    "             # characters to indent
COMMENT_PREFIX = ";"        # assembler comment prefix
DIRECTIVE_PREFIX = "."      # prefix for assembler directives
DEFINE_DIRECTIVE = "db"     # assembler directive to define a byte
VDP_PREFIX = "23,0,"        # VDP prefix bytes to define a custom character
LABEL = "Gradient"          # label to use for gradient
LOADER = True               # whether to generate a loader function

# extract characters from input file
lines = 0
chars = []
for line in fileinput.input():
    line = line.strip()
    if line.startswith(INPUT_PREFIX):
        data = line[len(INPUT_PREFIX):].strip().split(",")
        chars.append(",".join(data[:8]))
        chars.append(",".join(data[8:]))
        lines += 1
        if lines == MAX_LINES:
            break

# output characters in appropriate format
print(f"{LABEL}:")
for i, char in enumerate(chars):
    prefix = COMMENT_PREFIX if i % (SKIP_CHARS + 1) else DIRECTIVE_PREFIX
    print(f"{INDENT}{prefix}{DEFINE_DIRECTIVE} {VDP_PREFIX} {char}")
    if i == 0:
        print(f"{LABEL}Stride: equ $ - {LABEL}")
print(f"{LABEL}Length: equ $ - {LABEL}")
print(f"{LABEL}Count: equ {LABEL}Length / {LABEL}Stride")

# Assembly routine to load the custom chars
if LOADER:
    print(f"""{LABEL}Start: equ {START_CHAR}

; Load {LABEL} characters into VDU
Load{LABEL}:
    ld hl, {LABEL}
    inc hl                      ; start at second byte of first character
    ld de, {LABEL}Stride
    ld a, {LABEL}Start
    ld b, {LABEL}Count
Load{LABEL}Loop:
    ld (hl), a                  ; set character number
    inc a                       ; increment character number
    add hl, de
    djnz Load{LABEL}Loop
    ld hl, {LABEL}
    ld bc, {LABEL}Length
    rst.lil $18
    ret
""")