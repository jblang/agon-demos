; Palette Gradient Tools
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

; =============================================================================
; Constants

GradientChars: equ 256
SegmentCount: equ GradientChars/GradientCount

TileWidth: equ 8
TileHeight: equ GradientStride
TileSize: equ TileWidth*TileHeight


; =============================================================================
; Scratch area

TempTileBuffer:
    defs TileSize
TempTileBufferLength: equ $ - TempTileBuffer

TempColorBuffer:
    defs SegmentCount*2
TempColorBufferLength: equ $ - TempColorBuffer


; =============================================================================
; Macros

    ; Store the foreground + background pair in a color ramp
    macro LoadColor dest, index
        push hl
        ld e, index
        add hl, de
        ld a, (hl)
        ld dest, a
        pop hl
    endmacro


; =============================================================================
; Routines
    
; Generate foreground and background pairs from Plascii-style color scheme
; HL = pointer to RGBA2222 color definitions
; IX = pointer to palette
MakeColorRamp:
    ld iy, TempColorBuffer
    ld de, 0
    ld b, SegmentCount/2
MakeColorRampAsc:               ; load colors into lookup in ascending order
    LoadColor (iy+0), (ix+0)            ; foreground
    LoadColor (iy+SegmentCount), (ix+1) ; background
    inc ix
    inc iy
    djnz MakeColorRampAsc
    ld b, SegmentCount/2
MakeColorRampDesc:              ; load colors into lookup in descending order
    LoadColor (iy+0), (ix+0)            ; foreground
    LoadColor (iy+SegmentCount), (ix-1) ; background
    dec ix
    inc iy
    djnz MakeColorRampDesc
    ret


; Show all the gradient bitmaps, one after the other
ShowGradientBitmaps:
    xor a
ShowGradientBitmapsLoop:
    call SendChar
    inc a
    jp nz, ShowGradientBitmapsLoop
    ret


; Map all characters to selected gradient bitmaps
; Params:
;   DE = starting bitmap ID
SetGradient:
    push bc
    push de
    ld c, 0
SetGradientLoop:
    call MapCharToBitmap
    inc de
    inc c
    jp nz, SetGradientLoop
    pop de
    pop bc
    ret


; Load a single segment of the gradient with specified foreground/background color
; Params:
;   DE = bitmap number
;   B = background color
;   C = foreground color
LoadGradientSegment:
    ld hl, Gradient
    ld a, GradientCount
LoadGradientSegmentLoop:
    push de
    ld de, TempTileBuffer
    call MakeTile
    pop de
    push hl
    push bc
    ld hl, TempTileBuffer
    ld bc, TempTileBufferLength
    call SendTile
    pop bc
    pop hl
    inc de
    dec a
    jp nz, LoadGradientSegmentLoop
    ret

; Load bitmaps for gradient of the currently loaded colors
LoadGradientBitmaps:
    ld ix, TempColorBuffer
    ld b, 8
LoadGradientBitmapsLoop:
    push bc
    ld c, (ix+0)
    ld b, (ix+8)
    call LoadGradientSegment
    inc ix
    pop bc
    djnz LoadGradientBitmapsLoop
    ret


; Send the 8x8 RGBA2222 tile bitmap from the source buffer to the VDP
; Params:
;   HL = bitmap source buffer address
;   BC = bitmap length
;   DE = bitmap ID
SendTile:
    call ClearBuffer
    call SendBufferBlock
    call SelectBitmapBuffer
    push bc
    push de
    push hl
    ld bc, 8
    ld de, 8
    ld l, VduBitmapFormatRGBA2222
    call CreateBitmapFromBuffer
    pop hl
    pop de
    pop bc
    ret

; Make an 8x8 RGBA2222 bitmap with specified colors from an 8x8 mono bitmap 
; Params:
;   HL = pointer to monochrome bitmap
;   DE = tile output buffer
;   B = background color
;   C = foreground color
MakeTile:
    push af
    ld a, TileHeight
MakeTileRowLoop:
    push af
    ld a, (hl)
    call MakeTileRow
    pop af
    inc hl
    dec a
    jp nz, MakeTileRowLoop
    pop af
    ret


; make a single row of bytes in an 8x8 tile bitmap
; DE = output pointer
; A = bitmap
; B = background color in RGBA2222 format
; C = foreground color in RGBA2222 format
MakeTileRow:
    ex de, hl
    push de
    ld d, TileWidth
MakeTileColLoop:
    add a, a
    ld (hl), b
    jp c, MakeTileNextCol
    ld (hl), c
MakeTileNextCol:
    inc hl
    dec d
    jp nz, MakeTileColLoop
    pop de
    ex de, hl
    ret