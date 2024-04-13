; 64 user-defined gradient characters for AgonLight

; Shamelessly ripped from Produkthandler Kom Her by Camelot:
; 1) Download and unpack the following:
;    - Produkthandler Kom Her: https://csdb.dk/release/?id=760
;    - VICE: https://vice-emu.sourceforge.io/index.html#download (GTK3 version)
;    - CharPad: https://csdb.dk/release/?id=200341
; 2) Run demo in VICE; wait/press space until plasma section starts
; 3) Save a snapshot image
; 4) Import the snapshot image from step 3 in CharPad
; 5) Select "Character Set: 1 ($0800-$0FFF)"; click OK
; 6) Export characters to text/asm as "gradient.inc"
; 7) Delete everything except the first 32 .byte lines (64 chars; 2 per line)
; 8) Split into 8 bytes per line (1 char per line); change .byte to .db
; 9) Prefix each line with VDU load character command (23), GradientStart+n (character number)
; 10) Define labels; write LoadGradient and DrawGradient procedures

GradientStart:  equ     128 ; starting character to load (must be <= 192)

; Gradient bitmaps
Gradient:
    .db 23, GradientStart+0, $00,$00,$00,$00,$00,$00,$00,$00
    .db 23, GradientStart+1, $00,$00,$08,$00,$00,$00,$04,$00
    .db 23, GradientStart+2, $00,$00,$10,$00,$40,$00,$04,$00
    .db 23, GradientStart+3, $00,$02,$10,$00,$40,$00,$04,$00
    .db 23, GradientStart+4, $00,$02,$10,$00,$40,$00,$04,$20
    .db 23, GradientStart+5, $00,$02,$10,$02,$40,$00,$04,$20
    .db 23, GradientStart+6, $40,$02,$10,$02,$40,$00,$04,$20
    .db 23, GradientStart+7, $40,$02,$10,$02,$40,$08,$04,$20
    .db 23, GradientStart+8, $40,$02,$10,$02,$40,$08,$05,$20
    .db 23, GradientStart+9, $40,$02,$10,$02,$40,$88,$05,$20
    .db 23, GradientStart+10, $40,$02,$10,$0a,$40,$88,$05,$20
    .db 23, GradientStart+11, $44,$02,$10,$0a,$40,$88,$05,$20
    .db 23, GradientStart+12, $44,$02,$10,$0a,$41,$88,$05,$20
    .db 23, GradientStart+13, $44,$02,$50,$0a,$41,$88,$05,$20
    .db 23, GradientStart+14, $44,$02,$50,$0a,$41,$a8,$05,$20
    .db 23, GradientStart+15, $44,$0a,$50,$0a,$41,$a8,$05,$20
    .db 23, GradientStart+16, $44,$8a,$50,$0a,$41,$a8,$05,$20
    .db 23, GradientStart+17, $44,$8a,$50,$0a,$41,$aa,$05,$20
    .db 23, GradientStart+18, $44,$8a,$50,$0a,$51,$aa,$05,$20
    .db 23, GradientStart+19, $44,$8a,$50,$0a,$51,$aa,$45,$20
    .db 23, GradientStart+20, $54,$8a,$50,$0a,$51,$aa,$45,$20
    .db 23, GradientStart+21, $54,$8a,$51,$0a,$51,$aa,$45,$20
    .db 23, GradientStart+22, $54,$8a,$51,$0a,$51,$aa,$45,$28
    .db 23, GradientStart+23, $54,$8a,$51,$2a,$51,$aa,$45,$28
    .db 23, GradientStart+24, $55,$8a,$51,$2a,$51,$aa,$45,$28
    .db 23, GradientStart+25, $55,$8a,$51,$2a,$55,$aa,$45,$28
    .db 23, GradientStart+26, $55,$8a,$51,$2a,$55,$aa,$45,$2a
    .db 23, GradientStart+27, $55,$8a,$55,$2a,$55,$aa,$45,$2a
    .db 23, GradientStart+28, $55,$8a,$55,$2a,$55,$aa,$45,$aa
    .db 23, GradientStart+29, $55,$8a,$55,$aa,$55,$aa,$45,$aa
    .db 23, GradientStart+30, $55,$8a,$55,$aa,$55,$aa,$55,$aa
    .db 23, GradientStart+31, $55,$aa,$55,$aa,$55,$aa,$55,$aa
    .db 23, GradientStart+32, $55,$aa,$55,$aa,$55,$aa,$55,$aa
    .db 23, GradientStart+33, $55,$aa,$55,$aa,$55,$aa,$75,$aa
    .db 23, GradientStart+34, $55,$ba,$55,$aa,$55,$aa,$75,$aa
    .db 23, GradientStart+35, $55,$ba,$55,$aa,$d5,$aa,$75,$aa
    .db 23, GradientStart+36, $d5,$ba,$55,$aa,$d5,$aa,$75,$aa
    .db 23, GradientStart+37, $d5,$ba,$55,$aa,$d5,$ae,$75,$aa
    .db 23, GradientStart+38, $d7,$ba,$55,$aa,$d5,$ae,$75,$aa
    .db 23, GradientStart+39, $d7,$ba,$55,$ae,$d5,$ae,$75,$aa
    .db 23, GradientStart+40, $d7,$ba,$55,$ae,$d5,$ae,$75,$ab
    .db 23, GradientStart+41, $d7,$ba,$55,$ae,$f5,$ae,$75,$ab
    .db 23, GradientStart+42, $df,$ba,$55,$ae,$f5,$ae,$75,$ab
    .db 23, GradientStart+43, $df,$ba,$55,$ae,$f5,$af,$75,$ab
    .db 23, GradientStart+44, $df,$ba,$55,$ae,$f5,$af,$75,$bb
    .db 23, GradientStart+45, $df,$fa,$55,$ae,$f5,$af,$75,$bb
    .db 23, GradientStart+46, $df,$fa,$55,$be,$f5,$af,$75,$bb
    .db 23, GradientStart+47, $df,$fa,$57,$be,$f5,$af,$75,$bb
    .db 23, GradientStart+48, $df,$fa,$57,$be,$f5,$af,$f5,$bb
    .db 23, GradientStart+49, $df,$fa,$57,$be,$f5,$af,$fd,$bb
    .db 23, GradientStart+50, $df,$fa,$77,$be,$f5,$af,$fd,$bb
    .db 23, GradientStart+51, $df,$fa,$77,$be,$f5,$ef,$fd,$bb
    .db 23, GradientStart+52, $df,$fa,$77,$bf,$f5,$ef,$fd,$bb
    .db 23, GradientStart+53, $df,$fa,$77,$bf,$f5,$ef,$fd,$bf
    .db 23, GradientStart+54, $df,$fa,$77,$bf,$fd,$ef,$fd,$bf
    .db 23, GradientStart+55, $df,$fa,$f7,$bf,$fd,$ef,$fd,$bf
    .db 23, GradientStart+56, $df,$fb,$f7,$bf,$fd,$ef,$fd,$bf
    .db 23, GradientStart+57, $df,$fb,$ff,$bf,$fd,$ef,$fd,$bf
    .db 23, GradientStart+58, $df,$fb,$ff,$bf,$fd,$ef,$fd,$ff
    .db 23, GradientStart+59, $df,$fb,$ff,$bf,$ff,$ef,$fd,$ff
    .db 23, GradientStart+60, $ff,$fb,$ff,$bf,$ff,$ef,$fd,$ff
    .db 23, GradientStart+61, $ff,$fb,$ff,$bf,$ff,$ef,$ff,$ff
    .db 23, GradientStart+62, $ff,$fb,$ff,$ff,$ff,$ef,$ff,$ff
    .db 23, GradientStart+63, $ff,$ff,$ff,$ff,$ff,$ef,$ff,$ff
GradientLength: equ $ - Gradient
GradientCount:  equ GradientLength / 10