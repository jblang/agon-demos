    include "agon.inc"
    include "utility.inc"
    include "data.inc"

Main:
    call Init
    call RGBA2222Test
    call ColorRampTest
    call TileTest
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
    call HexByte
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
    call HexBytes
    call NewLine
    call NewLine
    ret

RGBA2222Buffer:
    defs 16
RGBA2222BufferLength: equ $ - RGBA2222Buffer

ColorRampTestMessage:
    defb "Color Ramp for Palette:", CR, LF
    defb "FG: ", EOS

ColorRampBGMessage:
    defb CR, LF, "BG: ", EOS

ColorRampTest:
    ld hl, ColorRampTestMessage
    call StringOut
    ld hl, RGBA2222Buffer
    ld ix, ColorPalettes
    ld iy, ColorRampBuffer
    call MakeColorRamp
    ld hl, ColorRampBuffer
    ld b, 8
    call HexBytes
    push hl
    ld hl, ColorRampBGMessage
    call StringOut
    pop hl
    ld b, 8
    call HexBytes
    call NewLine
    call NewLine
    ret

ColorRampBuffer:
    defs 16

TileTestMessage:
    defb "Tile Generation Test:", CR, LF, EOS

TileTest:
    ld c, $ff
    ld b, $00
    ld hl, Gradient
    ld de, GradientLength/2
    add hl, de
    ld de, TempTileBuffer
    call MakeTile
    ld hl, TileTestMessage
    call StringOut
    ld hl, TempTileBuffer
    ld c, TempTileBufferLength/8
    ld b, 8
TileTestLoop:
    push bc
    call HexBytes
    call NewLine
    pop bc
    dec c
    jp nz, TileTestLoop
    call NewLine
    call NewLine
    ret



ScreenBuffer:   equ $ + ScreenSize