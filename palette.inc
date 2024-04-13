; C64 Palette RGB values from http://unusedino.de/ec64/technical/misc/vic656x/colors/
C64Palette:
    .db 19, $0, $FF, $00,$00,$00
    .db 19, $1, $FF, $FF,$FF,$FF
    .db 19, $2, $FF, $68,$37,$2B
    .db 19, $3, $FF, $70,$A4,$B2
    .db 19, $4, $FF, $6F,$3D,$86
    .db 19, $5, $FF, $58,$8D,$43
    .db 19, $6, $FF, $35,$28,$79
    .db 19, $7, $FF, $B8,$C7,$6F
    .db 19, $8, $FF, $6F,$4F,$25
    .db 19, $9, $FF, $43,$39,$00
    .db 19, $A, $FF, $9A,$67,$59
    .db 19, $B, $FF, $44,$44,$44
    .db 19, $C, $FF, $6C,$6C,$6C
    .db 19, $D, $FF, $9A,$D2,$84
    .db 19, $E, $FF, $6C,$5E,$B5
    .db 19, $F, $FF, $95,$95,$95
C64PaletteLength:   equ $ - C64Palette
C64PaletteCount:    equ C64PaletteLength / 5