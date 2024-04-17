; C64 Palette C64Palette values from https://www.c64-wiki.com/wiki/Color
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
ColorPalettes:
Pal00:  .db $0,$b,$c,$f,$1
ColorPaletteStride: equ $ - ColorPalettes
Pal01:  .db $0,$0,$0,$5,$5
Pal02:  .db $d,$3,$e,$4,$2
Pal03:  .db $9,$2,$4,$b,$6
Pal04:  .db $6,$b,$8,$2,$2
Pal05:  .db $a,$c,$e,$5,$5
Pal06:  .db $6,$b,$8,$a,$7
Pal07:  .db $d,$3,$c,$8,$2
Pal08:  .db $1,$3,$e,$4,$2
Pal09:  .db $d,$5,$b,$4,$a
Pal0a:  .db $3,$e,$b,$8,$a
Pal0b:  .db $a,$4,$6,$e,$3
Pal0c:  .db $a,$8,$9,$b,$e
ColorPaletteEnd:    equ $
ColorPaletteLength: equ $ - ColorPalettes
ColorPaletteCount:  equ ColorPaletteLength / ColorPaletteStride
