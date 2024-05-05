; Data for plasma demo (sources credited individually below)

; Gradient patterns from Produkthandler Kom Her by Camelot: https://csdb.dk/release/?id=760
Gradient:
    .db $00,$00,$00,$00,$00,$00,$00,$00
GradientStride: equ $ - Gradient
    ;db $00,$00,$08,$00,$00,$00,$04,$00
    .db $00,$00,$10,$00,$40,$00,$04,$00
    ;db $00,$02,$10,$00,$40,$00,$04,$00
    .db $00,$02,$10,$00,$40,$00,$04,$20
    ;db $00,$02,$10,$02,$40,$00,$04,$20
    .db $40,$02,$10,$02,$40,$00,$04,$20
    ;db $40,$02,$10,$02,$40,$08,$04,$20
    .db $40,$02,$10,$02,$40,$08,$05,$20
    ;db $40,$02,$10,$02,$40,$88,$05,$20
    .db $40,$02,$10,$0a,$40,$88,$05,$20
    ;db $44,$02,$10,$0a,$40,$88,$05,$20
    .db $44,$02,$10,$0a,$41,$88,$05,$20
    ;db $44,$02,$50,$0a,$41,$88,$05,$20
    .db $44,$02,$50,$0a,$41,$a8,$05,$20
    ;db $44,$0a,$50,$0a,$41,$a8,$05,$20
    .db $44,$8a,$50,$0a,$41,$a8,$05,$20
    ;db $44,$8a,$50,$0a,$41,$aa,$05,$20
    .db $44,$8a,$50,$0a,$51,$aa,$05,$20
    ;db $44,$8a,$50,$0a,$51,$aa,$45,$20
    .db $54,$8a,$50,$0a,$51,$aa,$45,$20
    ;db $54,$8a,$51,$0a,$51,$aa,$45,$20
    .db $54,$8a,$51,$0a,$51,$aa,$45,$28
    ;db $54,$8a,$51,$2a,$51,$aa,$45,$28
    .db $55,$8a,$51,$2a,$51,$aa,$45,$28
    ;db $55,$8a,$51,$2a,$55,$aa,$45,$28
    .db $55,$8a,$51,$2a,$55,$aa,$45,$2a
    ;db $55,$8a,$55,$2a,$55,$aa,$45,$2a
    .db $55,$8a,$55,$2a,$55,$aa,$45,$aa
    ;db $55,$8a,$55,$aa,$55,$aa,$45,$aa
    .db $55,$8a,$55,$aa,$55,$aa,$55,$aa
    ;db $55,$aa,$55,$aa,$55,$aa,$55,$aa
    .db $55,$aa,$55,$aa,$55,$aa,$55,$aa
    ;db $55,$aa,$55,$aa,$55,$aa,$75,$aa
    .db $55,$ba,$55,$aa,$55,$aa,$75,$aa
    ;db $55,$ba,$55,$aa,$d5,$aa,$75,$aa
    .db $d5,$ba,$55,$aa,$d5,$aa,$75,$aa
    ;db $d5,$ba,$55,$aa,$d5,$ae,$75,$aa
    .db $d7,$ba,$55,$aa,$d5,$ae,$75,$aa
    ;db $d7,$ba,$55,$ae,$d5,$ae,$75,$aa
    .db $d7,$ba,$55,$ae,$d5,$ae,$75,$ab
    ;db $d7,$ba,$55,$ae,$f5,$ae,$75,$ab
    .db $df,$ba,$55,$ae,$f5,$ae,$75,$ab
    ;db $df,$ba,$55,$ae,$f5,$af,$75,$ab
    .db $df,$ba,$55,$ae,$f5,$af,$75,$bb
    ;db $df,$fa,$55,$ae,$f5,$af,$75,$bb
    .db $df,$fa,$55,$be,$f5,$af,$75,$bb
    ;db $df,$fa,$57,$be,$f5,$af,$75,$bb
    .db $df,$fa,$57,$be,$f5,$af,$f5,$bb
    ;db $df,$fa,$57,$be,$f5,$af,$fd,$bb
    .db $df,$fa,$77,$be,$f5,$af,$fd,$bb
    ;db $df,$fa,$77,$be,$f5,$ef,$fd,$bb
    .db $df,$fa,$77,$bf,$f5,$ef,$fd,$bb
    ;db $df,$fa,$77,$bf,$f5,$ef,$fd,$bf
    .db $df,$fa,$77,$bf,$fd,$ef,$fd,$bf
    ;db $df,$fa,$f7,$bf,$fd,$ef,$fd,$bf
    .db $df,$fb,$f7,$bf,$fd,$ef,$fd,$bf
    ;db $df,$fb,$ff,$bf,$fd,$ef,$fd,$bf
    .db $df,$fb,$ff,$bf,$fd,$ef,$fd,$ff
    ;db $df,$fb,$ff,$bf,$ff,$ef,$fd,$ff
    .db $ff,$fb,$ff,$bf,$ff,$ef,$fd,$ff
    ;db $ff,$fb,$ff,$bf,$ff,$ef,$ff,$ff
    .db $ff,$fb,$ff,$ff,$ff,$ef,$ff,$ff
    ;db $ff,$ff,$ff,$ff,$ff,$ef,$ff,$ff
GradientLength: equ $ - Gradient
GradientCount: equ GradientLength / GradientStride
GradientStart: equ 128

; C64 Palette RGB values from https://www.c64-wiki.com/wiki/Color
C64PaletteRGB:
    .db $00,$00,$00    ; black
C64PaletteRGBStride:   equ $ - C64PaletteRGB
    .db $ff,$ff,$ff    ; white
    .db $88,$00,$00    ; red
    .db $aa,$ff,$ee    ; cyan
    .db $cc,$44,$cc    ; purple
    .db $00,$cc,$55    ; green
    .db $00,$00,$aa    ; blue
    .db $ee,$ee,$77    ; yellow
    .db $dd,$88,$55    ; orange
    .db $66,$44,$00    ; brown
    .db $ff,$77,$77    ; light red
    .db $33,$33,$33    ; dark grey
    .db $77,$77,$77    ; grey
    .db $aa,$ff,$66    ; light green
    .db $00,$88,$ff    ; light blue
    .db $bb,$bb,$bb    ; light grey
C64PaletteRGBLength: equ $ - C64PaletteRGB
C64PaletteRGBCount: equ C64PaletteRGBLength / C64PaletteRGBStride

C64PaletteRGBA2222:
    .db $c0             ; black
    .db $ff             ; white
    .db $c2             ; red
    .db $fe             ; cyan
    .db $e2             ; purple
    .db $d8             ; green
    .db $e0             ; blue
    .db $df             ; yellow
    .db $db             ; orange
    .db $c6             ; brown
    .db $d7             ; light red
    .db $d5             ; dark grey
    .db $ea             ; grey
    .db $ce             ; light green
    .db $f4             ; light blue
    .db $ea             ; light grey
C64PaletteRGBA2222Length: equ $ - C64PaletteRGBA2222

; mapping of C64 colors to nearest approximation on TMS9918A
C64PaletteTMS:
    .db $01     ; black
    .db $0f     ; white
    .db $06     ; red
    .db $07     ; cyan
    .db $0d     ; purple
    .db $02     ; green -> medium green
    .db $04     ; blue
    .db $0b     ; yellow
    .db $09     ; orange -> light red
    .db $0a     ; brown -> dark yellow
    .db $08     ; light red -> medium red
    .db $01     ; dark grey -> black
    .db $0e     ; grey
    .db $03     ; light green
    .db $05     ; light blue
    .db $0e     ; light grey -> grey

; Predefined palettes from Plascii Petsma: https://csdb.dk/release/?id=159933
ColorPalettes:
    .db $0,$b,$c,$f,$1
ColorPaletteStride: equ $ - ColorPalettes
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
ColorPaletteEnd:    equ $
ColorPaletteLength: equ $ - ColorPalettes
ColorPaletteCount:  equ ColorPaletteLength / ColorPaletteStride

; Pre-defined plasma parameters from Plascii Petsma: https://csdb.dk/release/?id=159933
PlasmaParamList:
        defb    $fa,$05,$03,$fa,$07,$04,$fe,$fe
        defb    $fe,$01,$fe,$02,$03,$ff,$02,$02
        defb    $5e,$e8,$eb,$32,$69,$4f,$0a,$41
        defb    $fe,$fc
        defb    $06,$07
        defb    $ff
        defb    $01

        defb    $04,$05,$fc,$02,$fc,$03,$02,$01
        defb    $00,$01,$03,$fd,$02,$fd,$fe,$00
        defb    $51,$a1,$55,$c1,$0d,$5a,$dd,$26
        defb    $fe,$fd
        defb    $08,$08
        defb    $f8
        defb    $06

        defb    $f9,$06,$fe,$fa,$fa,$00,$07,$fb
        defb    $02,$01,$02,$03,$03,$00,$fd,$00
        defb    $34,$85,$a6,$11,$89,$2b,$fa,$9c
        defb    $fc,$fb
        defb    $09,$08
        defb    $fa
        defb    $09

        defb    $00,$01,$03,$00,$01,$ff,$04,$fc
        defb    $01,$ff,$03,$fe,$fe,$03,$02,$02
        defb    $f3,$02,$0b,$89,$8c,$d3,$23,$aa
        defb    $fe,$01
        defb    $07,$07
        defb    $08
        defb    $0a

        defb    $04,$04,$04,$fc,$fd,$04,$ff,$fc
        defb    $01,$02,$02,$01,$ff,$00,$ff,$01
        defb    $3a,$21,$53,$93,$39,$b7,$26,$99
        defb    $fd,$fe
        defb    $05,$06
        defb    $03
        defb    $04

        defb    $fd,$fd,$fd,$02,$04,$00,$fd,$02
        defb    $03,$02,$fd,$02,$03,$fe,$ff,$ff
        defb    $bc,$99,$5d,$2f,$e6,$16,$af,$0e
        defb    $fd,$ff
        defb    $07,$07
        defb    $f5
        defb    $07

        defb    $fc,$00,$00,$ff,$04,$04,$00,$01
        defb    $fd,$03,$00,$02,$00,$03,$02,$03
        defb    $30,$c7,$07,$60,$36,$2b,$e8,$ec
        defb    $ff,$fe
        defb    $09,$03
        defb    $f8
        defb    $05

        defb    $fd,$fc,$fe,$00,$00,$04,$fe,$01
        defb    $03,$03,$fe,$02,$00,$03,$fe,$00
        defb    $21,$d7,$34,$1b,$5d,$eb,$8e,$7d
        defb    $fd,$ff
        defb    $0a,$03
        defb    $fd
        defb    $03

        defb    $fe,$00,$ff,$01,$04,$02,$fe,$fd
        defb    $02,$01,$fe,$01,$03,$ff,$03,$ff
        defb    $0b,$0f,$ea,$8c,$e0,$f8,$05,$0e
        defb    $fc,$fd
        defb    $07,$06
        defb    $f8
        defb    $0c

        defb    $33,$04,$34,$fc,$dd,$24,$cf,$7c
        defb    $c1,$73,$02,$31,$fe,$a0,$ee,$01
        defb    $3a,$21,$53,$93,$39,$b7,$26,$99
        defb    $00,$00
        defb    $04,$01
        defb    $fd
        defb    $00

        defb    $ff,$00,$01,$ff,$02,$fe,$00,$02
        defb    $ff,$02,$01,$02,$fe,$01,$00,$00
        defb    $1d,$bb,$c5,$a3,$ab,$6c,$ed,$a6
        defb    $fd,$fe
        defb    $03,$03
        defb    $f8
        defb    $08

        defb    $02,$03,$fd,$fd,$01,$fc,$fd,$00
        defb    $01,$03,$fd,$fe,$fe,$03,$00,$00
        defb    $69,$ac,$3b,$c1,$fe,$21,$37,$84
        defb    $fc,$fd
        defb    $06,$05
        defb    $fa
        defb    $0b
LastPlasmaParam:

; Sine table contains pre-computed sine values converted to 8-bit integers.  
; Real sine values from -1 to 1 correspond to unsigned integers from 0 to 255.
; The first quarter of the period is pre-computed using python script:

; #!/usr/bin/python3
; import math
; amp = 0xfe
; for i in range(0, 0x40):
;     sin = 2 + amp / 2 + amp * 0.499999 * math.sin(i / (0x100 / 2 / math.pi))
;     if i & 7 == 0:
;         print("defb    ", end="")
;     print(hex(int(sin)).replace("0x", "$"), end="\n" if i & 7 == 7 else ",")

SineSrc:
        defb    $81,$84,$87,$8a,$8d,$90,$93,$96
        defb    $99,$9c,$9f,$a2,$a5,$a8,$ab,$ae
        defb    $b1,$b4,$b7,$ba,$bc,$bf,$c2,$c4
        defb    $c7,$ca,$cc,$cf,$d1,$d3,$d6,$d8
        defb    $da,$dc,$df,$e1,$e3,$e5,$e7,$e8
        defb    $ea,$ec,$ed,$ef,$f1,$f2,$f3,$f5
        defb    $f6,$f7,$f8,$f9,$fa,$fb,$fc,$fc
        defb    $fd,$fe,$fe,$ff,$ff,$ff,$ff,$ff