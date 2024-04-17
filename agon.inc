; VDP and MOS-specific routines for Plasma Generator on AgonLight, Agon Console8
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

ScreenMode:     equ 9                           ; 640x480 16 colors
ScreenWidth:    equ 40
ScreenHeight:   equ 30
ScreenSize:     equ ScreenWidth*ScreenHeight
PageMask:       equ $ffff00                     ; 24-bit addresses

; convenience macros

    macro DefPointer addr
        .dl addr
    endmacro

; executable preamble
    
    .assume adl=1                           ; ez80 ADL memory mode
    .org $40000                             ; load code here

    jp Entry                                ; jump to start of code

    .align 64                               ; MOS header
    .db "MOS",0,1     

    include "mos.inc"
    include "vdu.inc"
    include "gradient.inc"

Entry:
    push af                                 ; store all the registers
    push bc
    push de
    push ix
    push iy

    jp Main

Exit:
    call DefaultTextColor
    pop iy                                  ; pop all registers back from the stack
    pop ix
    pop de
    pop bc
    pop af
    ld hl,0                                 ; load the MOS API return code (0) for no errors.
    ret                                     ; return to MOS

EnableInterrupt:
    ret                     ; no interrupt needed on Agon

ShowHelp:
    ret                     ; figure out how to do this later