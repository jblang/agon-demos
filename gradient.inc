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

BitmapCounter:
    defw 0


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
    call EscapeChar
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


LoadPalette:
    ld de, 0
    ld a, (ColorPalette)
    ld d, a
    call SetGradient
    inc a
    ret


; Load a single segment of the gradient with specified foreground/background color
; Params:
;   B = background color
;   C = foreground color
LoadGradientSegment:
    ld hl, Gradient
    ld a, GradientCount
LoadTileExpandLoop:
    ld de, TempTileBuffer
    call MakeTile
    push hl
    push bc
    ld hl, TempTileBuffer
    ld bc, TempTileBufferLength
    ld de, (BitmapCounter)
    call SendTile
    pop bc
    pop hl
    inc de
    ld (BitmapCounter), de
    dec a
    jp nz, LoadTileExpandLoop
    ret

; Load bitmaps for gradient of the currently loaded colors
; Params:
;   DE = starting bitmap number
;   HL = pointer to ARGB2222 palette
;   IX = pointer to color scheme
LoadGradientBitmaps:
    call MakeColorRamp
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


LoadAllGradientsMessage:
    defb "Loading color gradients", NULL

LoadAllGradients:
    ld hl, LoadAllGradientsMessage
    call StringOut
    ld hl, C64PaletteRGBA2222
    ld ix, ColorPalettes
    ld b, ColorPaletteCount
LoadAllGradientsLoop:
    push bc
    push hl
    push ix
    call LoadGradientBitmaps
    pop ix
    pop hl
    ld bc, ColorPaletteStride
    add ix, bc
    VduSendByte '.'
    pop bc
    djnz LoadAllGradientsLoop
    call NewLine
    ret

; Send the 8x8 RGBA2222 tile bitmap from the source buffer to the VDP
; Params:
;   HL = bitmap source buffer address
;   BC = bitmap length
;   DE = bitmap ID
SendTile:
    call VduBufferClear
    call VduBufferWrite
    call SelectBitmapBuffer
    push bc
    push de
    push hl
    ld bc, 8
    ld de, 8
    ld l, VDU_BitmapFormatRGBA2222
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

GradientSetup:
    ; load all the buffers in the buffer load table
    ld ix, GradientBufferTable
    ld bc, GradientBufferTableStride
    ld a, GradientBufferTableCount
GradientSetupLoop:
    push bc
    ld de, (ix+0)
    ld hl, (ix+3)
    ld bc, (ix+6)
    call VduBufferClearAndWrite
    pop bc
    add ix, bc
    dec a
    jp nz, GradientSetupLoop
    ; split temp buffer into separate tile pattern buffers
    ld de, TempBufferID
    ld hl, TilePatternStartID
    ld bc, GradientStride
    call VduBufferSplitByWidthStartingAtID
    ret

; H = color for 1 bits
; L = color for 0 bits
GradientLoadSegment:
    ld (TileColors), hl
    push hl
    push bc
    push de
    ld hl, TileColors
    ld bc, TileColorsLength
    ld de, TempBufferID
    call VduBufferClearAndWrite
    ld de, TileExpandLoopID
    call VduBufferCall
    pop bc
    pop hl
    pop de
    ret

TileBufferStartID: equ 0
TempBufferID: equ $1000
TileExpandID: equ $1001
TileExpandLoopID: equ $1002
TileCreateID: equ $1003
TileCreateLoopID: equ $1004
TileCreateStartID: equ $1005
TileMapID: equ $1006
TileMapLoopID: equ $1007
TileMapStartID: equ $1008
TilePatternStartID: equ $1100

; table of buffer ID, starting address, and length to load
GradientBufferTable:
    dw24 TempBufferID, Gradient, GradientLength
GradientBufferTableStride: equ $ - GradientBufferTable
    dw24 TileExpandID, TileExpand, TileExpandLength
    dw24 TileExpandLoopID, TileExpandLoop, TileExpandLoopLength
    dw24 TileCreateID, TileCreate, TileCreateLength
    dw24 TileCreateLoopID, TileCreateLoop, TileCreateLoopLength
    dw24 TileCreateStartID, TileCreateStart, TileCreateStartLength
    dw24 TileMapID, TileMap, TileMapLength
    dw24 TileMapLoopID, TileMapLoop, TileMapLoopLength
    dw24 TileMapStartID, TileMapStart, TileMapStartLength
GradientBufferTableLength: equ $ - GradientBufferTable
GradientBufferTableCount: equ GradientBufferTableLength / GradientBufferTableStride

TileColors:
TileColor0:
    defb $00
TileColor1:
    defb $ff
TileColorsLength: equ $ - TileColors

; expand the source buffer into the target buffer using colors in temp buffer
TileExpand:
    defb VDU_Command, VDU_SystemCommand, VDU_BufferCommand
TileExpandDest: equ $ - TileExpand
    defw TileBufferStartID
    defb VDU_BufferBitmapExpand
    defb 1 | VDU_BufferBitmapExpandMappingBufferBit
TileExpandSource: equ $ - TileExpand
    defw TilePatternStartID
    defw TempBufferID
TileExpandLength: equ $ - TileExpand

; expand all the tiles in a gradient segment
TileExpandLoop:
    VduProgCall TileExpandID
    VduProgAdjust TileExpandID, TileExpandDest, VDU_BufferOpAdd, 1
    VduProgAdjust TileExpandID, TileExpandSource, VDU_BufferOpAdd, 1
    VduProgJumpIf TileExpandLoopID, TileExpandID, TileExpandSource, VDU_BufferCondLE, GradientCount
    VduProgAdjust TileExpandID, TileExpandSource, VDU_BufferOpSet, 0
TileExpandLoopLength: equ $ - TileExpandLoop

; create a bitmap from the buffer containing the 8x8 RGBA2222 tile definition
TileCreate:
    defb VDU_Command, VDU_BitmapCommand, VDU_BitmapSelectBuffer
TileCreateSource: equ $ - TileExpand
    defw TileBufferStartID
    defb VDU_Command, VDU_BitmapCommand, VDU_BitmapCreateBuffer
    defw 8
    defw 8
    defb VDU_BitmapFormatRGBA2222
TileCreateLength: equ $ - TileCreate

TileCreateLoop:
    VduProgCall TileCreateID
    VduProgAdjust TileCreateID, TileCreateSource, VDU_BufferOpAdd, 1
    VduProgJumpIf TileCreateLoopID, TileCreateID, TileCreateSource, VDU_BufferCondNE, 0
TileCreateLoopLength: equ $ - TileCreateLoop

SetFromBuffer: equ VDU_BufferOpSet | VDU_BufferFlagFetchOperand | VDU_BufferFlagMultipleTarget

; initialize the starting buffer and character for tile mapping loop
TileCreateStart:
    ;TODO: VduProgAdjust TileCreateID, TileCreateSource, SetFromBuffer, TempBufferID
    VduProgCall TileCreateLoopID
TileCreateStartLength: equ $ - TileCreateStart

; map a bitmap to a character number
TileMap:
    defb VDU_Command, VDU_SystemCommand, VDU_MapCharToBitmap
TileMapChar: equ $ - TileMap
    defb 0
TileMapBitmap: equ $ - TileMap
    defw TileBufferStartID
TileMapLength: equ $ - TileMap

; map all 256 characters to the corresponding bitmap
TileMapLoop:
    VduProgCall TileMapID
    VduProgAdjust TileMapID, TileMapChar, VDU_BufferOpAdd, 1
    VduProgJumpIf TileMapLoopID, TileMapID, TileMapChar, VDU_BufferCondNE, 0
TileMapLoopLength: equ $ - TileMapLoop

; initialize the starting buffer and character for tile mapping loop
TileMapStart:
    ; TODO: VduProgAdjust TileMapID, TileMapBitmap, SetFromBuffer, TempBufferID
    VduProgAdjust TileMap, TileMapChar, VDU_BufferOpSet, 0
    VduProgCall TileMapLoopID
TileMapStartLength: equ $ - TileMapStart