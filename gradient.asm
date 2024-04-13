; AgonLight Gradient Demo
; Copyright 2024 J.B. Langston
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

; Credits:
; - Gradients from Produkthandler Kom Her by Camelot: https://csdb.dk/release/?id=760
; - usg.asm lesson by Richard Turnnidge: https://github.com/richardturnnidge/lessons
    include "agonmacs.inc"

    .assume adl=1                       ; ez80 ADL memory mode
    .org $40000                         ; load code here

    jp start                            ; jump to start of code

    .align 64                           ; MOS header
    .db "MOS",0,1     

start:
            
    push af                             ; store all the registers
    push bc
    push de
    push ix
    push iy

    VdpMode 0
    PaperColor 2
    PenColor 10
    SendBytes C64Palette, C64PaletteLength
    SendBytes Gradient, GradientLength
    call DrawGradients                  ; display all 64 characters

    pop iy                              ; pop all registers back from the stack
    pop ix
    pop de
    pop bc
    pop af
    ld hl,0                             ; load the MOS API return code (0) for no errors.
    ret                                 ; return to MOS

; Draws all 64 gradient characters
DrawGradients:
    ld d, 0
PaperColorLoop:
    PaperColor d
    ld e, 0
PenColorLoop:
    PenColor e
    ld a, GradientStart
    ld b, GradientCount
GradientCharLoop:
    rst.lil $10
    inc a
    djnz GradientCharLoop
    NewLine
    inc e
    ld a, $f
    and e
    jp nz, PenColorLoop
    inc d
    ld a, $f
    and d
    jp nz, PaperColorLoop
    ret

    include "gradient.inc"
    include "palette.inc"