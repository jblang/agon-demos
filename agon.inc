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

InitGraphics:
    ld c, ScreenMode
    call VdpMode
    call LoadGradient
    call LoadC64Palette
    call DefaultTextColor
    call TextCursorHide
    ;call EnableScrollProtection
    ret

LoadC64Palette:
    SendBytes C64Palette, C64PaletteLength
    ret

; Send all the tiles in the screen buffer
SendScreenBuffer:
    SendByte VduCursorHome
    ld hl, ScreenBuffer
    ld bc, ScreenSize-1
SendScreenBufferLoop:
    ld a, (hl)
    push bc
    push hl
    call ShowTile
    pop hl
    pop bc
    inc hl
    dec bc
    ld a, b
    or c
    jp nz, SendScreenBufferLoop
    ret

; Load Gradient characters into VDU
LoadGradient:
    ld hl, Gradient
    inc hl                      ; start at second byte of first character
    ld de, GradientStride
    ld a, GradientStart
    ld b, GradientCount
LoadGradientLoop:
    ld (hl), a                  ; set character number
    inc a                       ; increment character number
    add hl, de
    djnz LoadGradientLoop
    ld hl, Gradient
    ld bc, GradientLength
    rst.lil $18
    ret

; macro to allow loading colors in both ascending and descending order
    macro LoadColors pen, paper
        ld a, pen
        ld (de), a
        inc de
        ld a, paper
        add 128
        ld (de), a
        inc de
    endmacro

LoadPalette:
    ld ix, (ColorPalette)
    ; fallthrough

; Load a Plascii-style palette into the color lookup table
; IX = address of palette to load
LoadColorLookup:
    ld de, ColorLookup
    ld b, 4
LoadColorLookupAsc:             ; load colors into lookup in ascending order
    LoadColors (ix+1), (ix+0)
    inc ix
    djnz LoadColorLookupAsc
    ld b, 4
LoadColorLookupDesc:            ; load colors into lookup in descending order
    LoadColors (ix-1), (ix+0)
    dec ix
    djnz LoadColorLookupDesc
    ret

; Output gradients for all the palettes one after another to demonstrate them
ShowPalettes:
    ld ix, ColorPalettes
    ld b, ColorPaletteCount
ShowPalettesLoop:
    push ix
    push bc
    ld a, ColorPaletteCount
    sub b
    call HexOut
    call Space
    call LoadColorLookup
    call ShowGradient
    call DefaultTextColor
    call NewLine
    pop bc
    pop ix
    ld de, ColorPaletteStride
    add ix, de
    djnz ShowPalettesLoop
    ret

; Display a gradient to showcase the current palette
ShowGradient:
    ld a, 0
ShowGradientLoop:
    push af
    call ShowTile
    pop af
    inc a
    jp nz, ShowGradientLoop
    ret

; Show the appropriate gradient tile with the appropriate pen and paper color
; A = tile number
ShowTile:
    push af
    and GradientCount - 1   ; bottom bits select the character
    add GradientStart       ; plus starting character
    ld (TileChar), a        ; save tile character
    pop af
    and 256 - GradientCount ; top bits select the colors
    rrca                    ; shift into correct positon
    rrca
    rrca
    rrca
    ld hl, CurrentColor
    cp (hl)                 ; check if this tile is already using the current color
    jp nz, ShowTileColor    ; if not, output color control codes
    ld a, (TileChar)        ; if so, just output the character
    rst.lis $10
    ret
ShowTileColor:
    ld (hl), a              ; save the current logical color
    ld hl, ColorLookup      ; look up the pen and paper color based on logical color
    ld b, 0
    ld c, a
    add hl, bc
    ld a, (hl)
    ld (TilePen), a         ; save them in the buffer
    inc hl
    ld a, (hl)
    ld (TilePaper), a
    SendBytes TileBuffer, TileBufferLength  ; send the tile buffer to the VDP
    ret

ColorLookup:
    .ds 16
CurrentColor:
    .db $ff                 ; ff is never a valid logical color
TileBuffer:
    .db VduTextColor
TilePen:
    .db 15                  ; white in default palette
    .db VduTextColor
TilePaper:
    .db VduBackgroundColor  ; black in defualt palette
TileChar:
    .db GradientStart
TileBufferLength: equ $ - TileBuffer

EnableInterrupt:
    ret                     ; no interrupt needed on Agon

ShowHelp:
    ret                     ; figure out how to do this later