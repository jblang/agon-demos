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

; platform specific graphics initialization
InitGraphics:
    ld c, ScreenMode
    call VdpMode
    call LoadGradient
    call LoadC64Palette
    call DefaultTextColor
    call TextCursorHide
    ;call EnableScrollProtection
    ret

    
; get a key press in A
GetKey:
    MosCall mos_sysvars         ; get ix pointer to sysvars
    ld a, (ix+sysvar_vkeydown)  ; get key state
    or a                        ; key is down; wait for it to be released
    jp nz, GetKey
    ld a, (ix+sysvar_keyascii)  ; load the ascii code of the key pressed
    ld (ix+sysvar_keyascii), 0  ; clear it for next time
    or a                        ; set z flag if key was pressed
    ret

; Load a data table into the VDU
; IX = table address
; HL = entry prefix function (0 for none)
; D = entry stride
; E = entry count
; BC = free for use in prefix function
LoadTable:
    call TablePrefix
    push de
LoadTableLoop:
    ld a, (ix+0)
    SendByteA
    inc ix
    dec d
    jp nz, LoadTableLoop
    pop de
    dec e
    jp nz, LoadTable
    ret  

; call table prefix function in HL, unless it is 0
; HL = function pointer
TablePrefix:
    ld a, h
    or l
    ret z
    jp (hl)

; load the C64 palette into agon palette
LoadC64Palette:
    ld ix, C64PaletteRGB
    ld hl, PalettePrefix
    ld c, 0
    ld d, C64PaletteRGBStride
    ld e, C64PaletteRGBCount
    jp LoadTable

PalettePrefix:
    SendByte VduSetPalette
    SendByte c
    SendByte VduSetPaletteRGB
    inc c
    ret

; load the gradient table into custom chracters
LoadGradient:
    ld ix, Gradient
    ld hl, GradientPrefix
    ld c, GradientStart
    ld d, GradientStride
    ld e, GradientCount
    jp LoadTable

GradientPrefix:
    SendByte VduCustomChar
    SendByte c
    inc c
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

    macro MakeTiles fg, bg
    push bc
    ld b, bg
    call RGBA2222
    ld b, a
    ld c, fg
    call RGBA2222
    ld c, a
    call MakeTiles
    pop bc
    endmacro

; Generate bitmap tiles from Plascii-style palette
; IX = pointer to palette
; HL = pointer to RGB color definitions
; DE = pointer to gradient tiles
MakeGradient:
    ld b, 4
MakeGradientAsc:             ; load colors into lookup in ascending order
    MakeTiles (ix+1), (ix+0)
    inc ix
    djnz MakeGradientAsc
    ld b, 4
MakeGradientDesc:            ; load colors into lookup in descending order
    dec ix
    MakeTiles (ix-1), (ix+0)
    djnz MakeGradientDesc
    ret

; return the RGBA2222 equivalent of RGBA bytes
; Params:
;   HL = pointer to RGB888 format color table
; Return:
;   A = RGBA2222 value
RGBA2222:
    push hl
    push de
    ld de, 3
RGBA2222Loop:
    add hl, de
    djnz RGBA2222Loop
    ld a, (hl)
    inc hl
    and %11000000
    ld d, a
    ld a, (hl)
    inc hl
    and %00110000
    rrca
    rrca
    ld a, 3
    ld a, (hl)
    inc hl
    and %00001100
    rrca
    rrca
    rrca
    rrca
    or d
    or e
    or %00000011
    pop de
    pop hl
    ret

MakeTiles:
    push af
    ex de, hl
    ld a, 32    
MakeTileLoop:
    call MakeTile
    call SendTile
    dec a
    jp nz, MakeTileLoop
    ex de, hl
    pop af
    ret

; send the tile currently in the temp buffer to the vdu
; DE = bitmap ID
SendTile:
    push af
    push hl
    push bc
    push de
    call ClearBuffer
    ld hl, TempTileBuffer
    ld bc, TempTileBufferLength
    call SendBufferBlock
    call SelectBitmapBuffer
    ld bc, 8
    ld de, 8
    ld h, VduBitmapFormatRGBA2222
    call CreateBitmapFromBuffer
    pop de
    pop bc
    pop hl
    pop af

; make a color tile with specified foreground, and background colors
; HL = bitmap input buffer
; B = background color
; C = foreground color
MakeTile:
    push af
    push hl
    push de
    ld a, 8
    ld de, TempTileBuffer
MakeTileRowLoop:
    push af
    ld a, (hl)
    call MakeTileRow
    pop af
    inc hl
    dec a
    jp nz, MakeTileRowLoop
    pop de
    pop hl
    pop af
    ret

; make a single row of bytes in a bitmap
; HL = output pointer
; A = bitmap
; B = background color in RGBA2222 format
; C = foreground color in RGBA2222 format
MakeTileRow:
    ld d, 8
MakeTileColLoop:
    add a, a
    ld (hl), b
    jp c, MakeTileNextCol
    ld (hl), c
MakeTileNextCol:
    inc hl
    dec d
    jp nz, MakeTileColLoop
    ret
TempTileBuffer:
    defs 64
TempTileBufferLength: equ $ - TempTileBuffer

; clear a buffer
; DE = buffer id
ClearBuffer:
    SendByte VduCommand
    SendByte VduSystem
    SendByte VduBuffer
    SendByte e                  ; buffer id
    SendByte d
    SendByte VduBufferClear
    ret

; send a block of data to a buffer
; DE = buffer id
; BC = buffer length
; HL = buffer data
SendBufferBlock:
    SendByte VduCommand
    SendByte VduSystem
    SendByte VduBuffer
    SendByte e                  ; buffer id
    SendByte d
    SendByte VduBufferWriteBlock
    SendByte c                  ; length
    SendByte b
SendBufferLoop:
    SendByte (hl)
    inc hl
    dec bc
    ld a, b
    or c
    jp nz, SendBufferLoop
    ret

; select a bitmap using buffer ID
; DE = buffer id
SelectBitmapBuffer:
    SendByte VduCommand
    SendByte VduBitmap
    SendByte VduBitmapSelectBuffer
    SendByte e                  ; buffer id
    SendByte d
    ret

; create selected bitmap from buffer
; BC = width
; DE = height
; L = format
CreateBitmapFromBuffer:
    push af
    SendByte VduCommand
    SendByte VduBitmap
    SendByte VduBitmapCreateBuffer
    SendByte c                  ; width
    SendByte b
    SendByte e                  ; height
    SendByte d
    SendByte l                  ; format
    ret

; map character number to bitmap using buffer id
; C = char
; DE = bitmap id
MapCharToBitmap:
    SendByte VduCommand
    SendByte VduSystem
    SendByte VduMapCharToBitmap
    SendByte c                  ; character number
    SendByte e                  ; bitmap id
    SendByte d
    ret

; send a character, escaped if necessary
; A = char to send
SendChar:
    cp ' '                      ; check if it's a control char
    jp nc, SendCharUnescaped    ; if not, send unescaped
    push af
    SendByte VduEscapeChar      ; if so, send escape command first
    pop af
SendCharUnescaped:
    SendByteA
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

; Load a Plascii-style palette into the color lookup table
; HL = address of palette to load
LoadColorTable:
    push hl
    pop ix                      ; palette address is now in ix
    ld de, ColorTable
    ld b, 4
LoadColorTableAsc:             ; load colors into lookup in ascending order
    LoadColors (ix+1), (ix+0)
    inc ix
    djnz LoadColorTableAsc
    ld b, 4
LoadColorTableDesc:            ; load colors into lookup in descending order
    LoadColors (ix-1), (ix+0)
    dec ix
    djnz LoadColorTableDesc
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
    call LoadColorTable
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
    ld hl, ColorTable       ; look up the pen and paper color based on logical color
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

ColorTable:
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