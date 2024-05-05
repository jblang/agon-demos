    include "agon.inc"
    include "utility.inc"
    include "data.inc"
    include "gradient.inc"

Main:
    call Init
    call RGBA2222Test
    call TileTest
    call ColorRampTest
    call GradientTest
    jp Exit

Banner:
    defb "Plasma Test Suite by J.B. Langston", CR, LF, CR, LF, EOS

Init:
    ld c, 3                     ; 640x480 16 colors
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
    defb "Color Ramp Generation Test:", CR, LF
    defb "FG: ", EOS

ColorRampBGMessage:
    defb CR, LF, "BG: ", EOS

ColorRampTest:
    ld hl, ColorRampTestMessage
    call StringOut
    ld hl, C64PaletteRGBA2222
    ld ix, ColorPalettes
    call MakeColorRamp
    ld hl, TempColorBuffer
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

GradientTestMessage:
    defb "Color Gradient Test:"

GradientCountMessage:
    defb " <- Gradient ", EOS

GradientNextMessage:
    defb "; press any key...", CR, LF, CR, LF, EOS

GradientTest:
    call LoadAllGradients
    ld de, 0
    ld b, ColorPaletteCount
GradientTestLoop:
    call SetGradient
    call ShowGradientBitmaps
    call ResetBitmapChars
    ld hl, GradientCountMessage
    call StringOut
    ld a, d
    call HexByte
    ld hl, GradientNextMessage
    call StringOut
    call WaitKey
    inc d
    djnz GradientTestLoop
    call NewLine
    ret

CountControlTestMessage:
    defb "Control character counting test (33 out of 256):", CR, LF, CR, LF, EOS

CountControlTest:
    ret

ColorPalette:
        defb    0

ScreenBuffer:   equ $ + ScreenSize