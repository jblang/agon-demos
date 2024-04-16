; C64 Palette C64Palette values from http://unusedino.de/ec64/technical/misc/vic656x/colors/

C64Palette:
    .db 19, $0, $ff, $00,$00,$00    ; C64 colors
C64PaletteStride:   equ $ - C64Palette
    .db 19, $1, $ff, $ff,$ff,$ff
    .db 19, $2, $ff, $88,$00,$00
    .db 19, $3, $ff, $aa,$ff,$ee
    .db 19, $4, $ff, $cc,$44,$cc
    .db 19, $5, $ff, $00,$cc,$55
    .db 19, $6, $ff, $00,$00,$aa
    .db 19, $7, $ff, $ee,$ee,$77
    .db 19, $8, $ff, $dd,$88,$55
    .db 19, $9, $ff, $66,$44,$00
    .db 19, $a, $ff, $ff,$77,$77
    .db 19, $b, $ff, $33,$33,$33
    .db 19, $c, $ff, $77,$77,$77
    .db 19, $d, $ff, $aa,$ff,$66
    .db 19, $e, $ff, $00,$88,$ff
    .db 19, $f, $ff, $bb,$bb,$bb 
C64PaletteLength: equ $ - C64Palette
C64PaletteCount: equ C64PaletteLength / C64PaletteStride

; Predefined palettes from Plascii Petsma: https://csdb.dk/release/?id=159933
Palettes:
    .db $0,$b,$c,$f,$1
PaletteStride: equ $ - Palettes
    .db $0,$0,$0,$5,$5
    .db $d,$3,$e,$4,$2
    .db $9,$2,$4,$b,$6
    .db $6,$b,$8,$2,$2
    .db $a,$c,$e,$5,$5
    .db $6,$b,$8,$a,$7
    .db $d,$3,$c,$8,$2
    .db $1,$3,$e,$4,$2
    .db $d,$5,$b,$4,$a
    .db $3,$e,$b,$8,$a
    .db $a,$4,$6,$e,$3
    .db $a,$8,$9,$b,$e
PaletteLength: equ $ - Palettes
PaletteCount: equ PaletteLength / PaletteStride

; Load a Plascii-style palette into the color lookup table
; IX = address of palette to load
LoadPalette:
    ld iy, ColorLookup
    ld de, GradientCount
    ld b, 4
LoadPaletteAsc:             ; Load colors into lookup in ascending order
    ld a, (ix+1)
    ld (iy+0), a
    ld a, (ix+0)
    add 128
    ld (iy+1), a
    add iy, de
    inc ix
    djnz LoadPaletteAsc
    ld b, 4
LoadPaletteDesc:            ; Load colors into lookup in descending order
    ld a, (ix-1)
    ld (iy+0), a
    ld a, (ix+0)
    add 128
    ld (iy+1), a
    add iy, de
    dec ix
    djnz LoadPaletteDesc
    ret

ShowPalettes:
    ld hl, C64Palette
    ld bc, C64PaletteLength
    rst.lil $18
    ld ix, Palettes
    ld b, PaletteCount
ShowPalettesLoop:
    push ix
    push bc
    call LoadPalette
    call ShowGradient
    NewLine
    pop bc
    pop ix
    ld de, PaletteStride
    add ix, de
    djnz ShowPalettesLoop


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
    ld b, a
    and GradientCount - 1   ; bottom 5 bits select the character
    add GradientStart
    ld (TileCharacter), a
    ld a, b
    and $e0                 ; top 3 bits select the colors
    ld hl, CurrentColor     ; check if this tile is already using the current color
    cp (hl)
    jp nz, ShowTileColor    ; if not, output color control codes
    ld a, (TileCharacter)   ; if so, just output the character
    rst.lil $10
    ret
ShowTileColor:
    ld (hl), a              ; Look up the physical pen and paper color based on logical color
    ld hl, ColorLookup
    ld b, 0
    ld c, a
    add hl, bc
    ld a, (hl)
    ld (TilePen), a
    inc hl
    ld a, (hl)
    ld (TilePaper), a
    ld hl, TileBuffer       ; Send the tile buffer to the VDP
    ld bc, TileBufferLength
    rst.lil $18
    ret
CurrentColor: .db $ff
TileBuffer:
    .db 17
TilePen:
    .db 15
    .db 17
TilePaper:
    .db 128
TileCharacter:
    .db 128
TileBufferLength: equ $ - TileBuffer

ColorLookup:
    .ds 256
