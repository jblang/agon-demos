; Sine utilties
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

; Ported from 6502 Sine Routines from Plascii Petsma by Camelot
; https://csdb.dk/release/?id=159933

; MakeSineTable builds the sine table for a complete period from a precalculated quarter period.
; The first 64 values are copied verbatim from the precomputed values. The next 64 values are
; flipped horizontally by copying them in reverse order. The last 128 values are flipped 
; vertically by complementing them. The vertically flipped values are written twice, first in
; forward order, and then in reverse order to flip them horizontally and complete the period.
; The resulting lookup table is 256 bytes long and stored on a 256-byte boundary so that a sine
; value can be looked up by loading a single register with the input value.

MakeSineTable:
        ld      bc, SineSrc             ; source values
        ld      de, SineTable           ; start of 1st quarter
        ld      hl, SineTable+$7f       ; end of 2nd quarter
        exx
        ld      b, $40                  ; counter
        ld      de, SineTable+$80       ; start of 3rd quarter
        ld      hl, SineTable+$ff       ; end of 4th quarter
SineLoop:
        exx
        ld      a, (bc)                 ; load source value
        inc     bc
        ld      (de), a                 ; store 1st quarter
        inc     de
        ld      (hl), a                 ; store 2nd quarter
        dec     hl                      ; in reverse order
        exx
        cpl                             ; flip vertically
        ld      (de), a                 ; store 3rd quarter
        inc     de
        ld      (hl), a                 ; store 4th quarter
        dec     hl                      ; in reverse order
        djnz    SineLoop
        ret

; Sine table contains pre-computed sine values converted to 8-bit integers.  
; Real sine values from -1 to 1 correspond to unsigned integers from 0 to 255.
; The first quarter of the period is pre-computed using python script:

; #!/usr/bin/python3
; import math
; amp = 0xfe
; for i in range(0, 0x40):
;     sin = 2 + amp / 2 + amp * 0.499999 * math.sin(i / (0x100 / 2 / math.pi))
;     if i & 7 == 0:
;         print("defb    ", end="")
;     print(hex(int(sin)).replace("0x", "$"), end="\n" if i & 7 == 7 else ",")

SineSrc:
        defb    $81,$84,$87,$8a,$8d,$90,$93,$96
        defb    $99,$9c,$9f,$a2,$a5,$a8,$ab,$ae
        defb    $b1,$b4,$b7,$ba,$bc,$bf,$c2,$c4
        defb    $c7,$ca,$cc,$cf,$d1,$d3,$d6,$d8
        defb    $da,$dc,$df,$e1,$e3,$e5,$e7,$e8
        defb    $ea,$ec,$ed,$ef,$f1,$f2,$f3,$f5
        defb    $f6,$f7,$f8,$f9,$fa,$fb,$fc,$fc
        defb    $fd,$fe,$fe,$ff,$ff,$ff,$ff,$ff