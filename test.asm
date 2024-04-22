    include "agon.inc"
    include "utility.inc"
    include "data.inc"

Main:
    call Init
    call RGBA2222Test
    call ColorPairTest
    jp Exit

Banner:
    defb "Plasma Test Suite by J.B. Langston", CR, LF, CR, LF, EOS

Init:
    ld c, 0                 ; 640x480 16 colors
    call VdpMode
    call ClearScreen
    ld hl, Banner
    call StringOut
    ret

RGBA2222TestMessage:
    defb "RGBA2222 Conversion: ", CR, LF, EOS

RGBA2222Test:
    ld hl, RGBA2222TestMessage
    call StringOut
    ld b, RGBA2222BufferLength
    xor a
RGBA2222TestLoop:
    push af
    call HexOut
    call Space
    pop af
    inc a
    djnz RGBA2222TestLoop
    call NewLine
    ld hl, C64PaletteRGB
    ld b, C64PaletteRGBCount
    ld de, RGBA2222Buffer
    call MakeRGBA2222
    ld hl, RGBA2222Buffer
    ld b, RGBA2222BufferLength
    call HexBytesOut
    call NewLine
    call NewLine
    ret

ColorPairTestMessage:
    defb "Color Pairs for Palette", CR, LF, EOS

ColorPairTest:
    ld hl, ColorPairTestMessage
    call StringOut
    ld hl, RGBA2222Buffer
    ld ix, ColorPalettes
    ld iy, ColorPairBuffer
    call MakeColorPairs
    ld hl, ColorPairBuffer
    ld b, 8
    call HexBytesOut
    call NewLine
    ld b, 8
    call HexBytesOut
    call NewLine
    ret

HexBytesOut:
    ld a, (hl)
    call HexOut
    call Space
    inc hl
    djnz HexBytesOut
    ret

RGBA2222Buffer:
    defs 16
RGBA2222BufferLength: equ $ - RGBA2222Buffer

ColorPairBuffer:
    defs 16

ScreenBuffer:   equ $ + ScreenSize