; Miscellaneuous utility functions 
; Copyright 2018-2024 J.B. Langston
;
; Permission is hereby granted, free of charge, to any person obtaining a 
; copy of this software and associated documentation files (the "Software"), 
; to deal in the Software without restriction, including without limitation 
; the rights to use, copy, modify, merge, publish, distribute, sublicense, 
; and/or sell copies of the Software, and to permit persons to whom the 
; Software is furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
; DEALINGS IN THE SOFTWARE.


        ; jumps to target label if the specified value is in A
        ; can be used to build a concise dispatch routine...
        ;
        ; Dispatch:
        ;       JumpIf 0, Target0
        ;       JumpIf 1, Target1
        ;       ; ...
        ;       ret
        macro JumpIf value, target
        cp      value
        jp      z, target
        endmacro


; CharOut must be defined elsewhere for HexByte and HexBytes to work

; Output a byte in hex
;       A = byte to output
HexByte:
        push    af
        rra
        rra
        rra
        rra
        call    HexNybble
        call    CharOut
        pop     af
        call    HexNybble
        jp      CharOut

; Output byte buffer in hex
;       HL = address of byte buffer
;       B = length of byte buffer
HexBytes:
        ld      a, (hl)
        call    HexByte
        call    Space
        inc     hl
        djnz    HexBytes
        ret

; convert lower nybble of A to hex (also in A)
; from http://map.grauw.nl/sources/external/z80bits.html#5.1
HexNybble:
        or      0f0h
        daa
        add     a, 0a0h
        adc     a, 40h
        ret

; RandomSeed sets the seed from four bytes in screen buffer data offset by refresh register.
RandomSeed:
        ld      hl, ScreenBuffer
        ld      a, r
        ld      d, 0
        ld      e, a
        add     hl, de
        ld      b, 4
        ld      de, Seed1
RandomSeedLoop:
        ld      a, (hl)
        xor     l
        ld      (de), a
        inc     hl
        inc     de
        djnz    RandomSeedLoop
        ret

; RandomNumber generates a random number using combined LFSR/LCG PRNG with 16-bit seeds
; https://wikiti.brandonw.net/index.php?title=Z80_Routines:Math:Random
RandomNumber:
        ld      hl, (Seed1)
        ld      b, h
        ld      c, l
        add     hl, hl
        add     hl, hl
        inc     l
        add     hl, bc
        ld      (Seed1), hl
        ld      hl, (Seed2)
        add     hl, hl
        sbc     a, a
        and     %00101101
        xor     l
        ld      l, a
        ld      (Seed2), hl
        add     hl, bc
        ret

Seed1:
        defw    0
Seed2:
        defw    0

; RandomSeries generates series of random numbers
; b = number of random numbers to generate
; c = mask for random numbers
; d = offset for random numbers
RandomSeries:
        push    bc
        push    hl
        call    RandomNumber
        ld      a, l
        or      a
        pop     hl
        pop     bc
        call    m, RandomNeg
        call    p, RandomPos
        ld      (hl), a
        inc     hl
        djnz    RandomSeries
        ret
RandomPos:
        and     c
        add     a, d
        ret
RandomNeg:
        and     c
        add     a, d
        cpl
        inc     a
        ret
