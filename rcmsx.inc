; TMS9918-specific routines for Plasma Generator on RC2014 and MSX
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

ScreenWidth:    equ 32
ScreenHeight:   equ 24
ScreenSize:     equ ScreenWidth*ScreenHeight
MSX:            equ 0
PageMask:       equ $ff00

; platform-specific preparation

        org     $100
        ld      (OldSP), sp
        ld      sp, Stack

        call    z180detect                      ; detect Z180
        ld      e, 0
        jp      nz, NoZ180
        call    z180getclk                      ; get clock multiple for tms wait
NoZ180:
        call    TmsSetWait                      ; set VDP wait loop based on clock multiple

        call    TmsProbe                        ; find what port TMS9918A listens on
        jp      z, NoTms
        call    TmsTile
        jp      Main

EnableInterrupt:
if MSX
        di
        ld      hl, ($39)
        ld      (MSXVector), hl
        ld      hl, MSXIntHandler
        ld      ($39), hl
        call    TmsIntEnable
        ei
endif
        ret

Exit:
        ld      sp, (OldSP)
if MSX  ; reset interrupt vector
        di
        ld      hl, (MSXVector)
        ld      ($39), hl
        ei
        ld      a, ($fcb0)
        rst     $30
        db      $80
        dw      $5f
endif
        rst     0

if MSX
FrameReady:
        defb    0
MSXIntHandler:
        push    af
        push    hl
        push    de
        push    bc
        ld      a, (FrameReady)
        or      a
        jp      z, FrameNotReady
        ld      de, (TmsNameAddr)
        call    TmsWriteAddr
        ld      hl, ScreenBuffer
        ld      bc, (TmsPort)
        ld      de, ScreenSize
        inc     d
MSXIntLoop:
        ld      a, (hl)
        out     (c), a
        inc     hl
        dec     e
        jp      nz, MSXIntLoop
        dec     d
        jp      nz, MSXIntLoop
        xor     a
        ld      (FrameReady), a
FrameNotReady:
        pop     bc
        pop     de
        pop     hl
        pop     af
MSXVector:      equ $+1
        jp      0

SendScreenBuffer:
        ret

WaitVSync:
        ld      a, $ff
        ld      (FrameReady), a
WaitVsyncLoop:
        ld      a, (FrameReady)
        or      a
        jp      nz, WaitVsync
        ret

else    ; RC2014

SendScreenBuffer:
        ld      hl, ScreenBuffer                ; display next frame
        ld      de, (TmsNameAddr)
        ld      bc, ScreenSize
        call    TmsWrite
        ret

WaitVSync:
        call    TmsRegIn
        and     $80
        jr      z, WaitVsync
        ret

endif

NoTmsMessage:
        defb    "TMS9918A not found, aborting!$"
NoTms:  ld      de, NoTmsMessage
        call    strout
        jp      Exit

ShowHelp:
if !MSX
        ld      de, Help
        call    strout
endif
        ret


; LoadColorTable sets up color table using current palette
;
; The color table in Graphics I mode consists of 32 bytes. Each byte defines two colors 
; for 8 consecutive patterns in the pattern table.  The upper nybble defines the color
; of the 1 bits and the lower nybble defines the color of the 0 bits. 
;
; For simplicity, palettes are stored with one color per byte, and the LoadColorTable 
; routine combines each adjacent color into a single byte for the color table. Since 
; we are using 8 colors and 32 tiles per color combination, we need to load each color
; combination into the color table 4 times.
LoadColorTable:
        ld      de, (TmsColorAddr)
        call    TmsWriteAddr
        ld      c, (hl)
        ld      d, c
        ld      e, PaletteLen-1
AddColorLoop:
        inc     hl
        ld      a, (hl)
        call    AddColors
        ld      c, (hl)
        dec     e
        jp      nz, AddColorLoop
        ld      a, d
        ; fallthrough
AddColors:
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        or      c
        ld      b, ColorRepeats
ColorRepeatLoop:
        call    TmsRamOut
        djnz    ColorRepeatLoop
        ret

; VIC-II to TMS9918 color mappings
; compromises with no direct mapping are marked with #
; vic:  $00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0a,$0b,$0c,$0d,$0e,$0f
; tms:  $01,$0f,$06,$07,$0d,$0c,$04,$0b,$0a,#0A,#09,#01,$0e,$03,$05,#0E

; palettes pre-mapped from vic to tms
ColorPalettes:
Pal00:  defb    $01,#01,$0e,#0e,$0f,#0e,$0e,#01
Pal01:  defb    $01,$01,$01,$02,$02,$02,$01,$01
Pal02:  defb    $03,$07,$05,$0d,$06,$0d,$05,$07
Pal03:  defb    #06,$08,$0d,#01,$04,#01,$0d,$08
Pal04:  defb    $04,#01,$0a,$08,$08,$08,$0a,#01
Pal05:  defb    $08,$01,$04,$02,$02,$02,$04,$01
Pal06:  defb    $04,#01,$06,$09,$0b,$09,$06,#01
Pal07:  defb    $03,$07,$01,$0a,$06,$0a,$01,$07
Pal08:  defb    $0f,$07,$05,$0d,$06,$0d,$05,$07
Pal09:  defb    $03,$0c,#01,$0d,$09,$0d,#01,$0c
Pal0a:  defb    $07,$05,#01,$06,$09,$06,#01,$05
Pal0b:  defb    $09,$0d,$04,$05,$07,$05,$04,$0d
Pal0c:  defb    $0b,$0a,#06,#01,$05,#01,#06,$0a
Pal0d:  defb    $08,$09,$0b,$03,$07,$05,$04,$0d
Pal0e:  defb    $01,$0c,$02,$03,$0f,$09,$08,$06
Pal0f:  defb    $01,$04,$05,$07,$0f,$0b,$0a,$0d
LastPalette:

; LoadPatternTable loads 8 copies of the 32 tiles into the TMS9918 pattern table.
LoadGradient:
        ld      de, (TmsPatternAddr)
        call    TmsWriteAddr
        ld      b, PatternRepeats
PatternRepeatLoop:
        ld      hl, Patterns
        ld      de, PatternLen
PatternLoop:
        ld      a, (hl)
        call    TmsRamOut
        inc     hl
        dec     de
        ld      a, d
        or      e
        jp      nz, PatternLoop
        djnz    PatternRepeatLoop
        ret

; Gradients from Produkthandler Kom Her by Camelot
; https://csdb.dk/release/?id=760

; The TMS9918 tile mode defines 256 tile patterns, each of which is associated with a specific
; foreground and background color. For palettes of 8 colors each, we can use 32 tiles per color,
; so we only use every other tile the set of 64 tiles.

Patterns:
        defb    $00,$00,$00,$00,$00,$00,$00,$00
        defb    $00,$00,$10,$00,$40,$00,$04,$00
        defb    $00,$02,$10,$00,$40,$00,$04,$20
        defb    $40,$02,$10,$02,$40,$00,$04,$20
        defb    $40,$02,$10,$02,$40,$08,$05,$20
        defb    $40,$02,$10,$0a,$40,$88,$05,$20
        defb    $44,$02,$10,$0a,$41,$88,$05,$20
        defb    $44,$02,$50,$0a,$41,$a8,$05,$20
        defb    $44,$8a,$50,$0a,$41,$a8,$05,$20
        defb    $44,$8a,$50,$0a,$51,$aa,$05,$20
        defb    $54,$8a,$50,$0a,$51,$aa,$45,$20
        defb    $54,$8a,$51,$0a,$51,$aa,$45,$28
        defb    $55,$8a,$51,$2a,$51,$aa,$45,$28
        defb    $55,$8a,$51,$2a,$55,$aa,$45,$2a
        defb    $55,$8a,$55,$2a,$55,$aa,$45,$aa
        defb    $55,$8a,$55,$aa,$55,$aa,$55,$aa
        defb    $55,$aa,$55,$aa,$55,$aa,$55,$aa
        defb    $55,$ba,$55,$aa,$55,$aa,$75,$aa
        defb    $d5,$ba,$55,$aa,$d5,$aa,$75,$aa
        defb    $d7,$ba,$55,$aa,$d5,$ae,$75,$aa
        defb    $d7,$ba,$55,$ae,$d5,$ae,$75,$ab
        defb    $df,$ba,$55,$ae,$f5,$ae,$75,$ab
        defb    $df,$ba,$55,$ae,$f5,$af,$75,$bb
        defb    $df,$fa,$55,$be,$f5,$af,$75,$bb
        defb    $df,$fa,$57,$be,$f5,$af,$f5,$bb
        defb    $df,$fa,$77,$be,$f5,$af,$fd,$bb
        defb    $df,$fa,$77,$bf,$f5,$ef,$fd,$bb
        defb    $df,$fa,$77,$bf,$fd,$ef,$fd,$bf
        defb    $df,$fb,$f7,$bf,$fd,$ef,$fd,$bf
        defb    $df,$fb,$ff,$bf,$fd,$ef,$fd,$ff
        defb    $ff,$fb,$ff,$bf,$ff,$ef,$fd,$ff
        defb    $ff,$fb,$ff,$ff,$ff,$ef,$ff,$ff
PatternLen:     equ $ - Patterns
NumPatterns:    equ PatternLen / 8
PatternRepeats: equ 256 / NumPatterns
ColorRepeats:   equ NumPatterns / 8
PaletteLen:     equ 32 / ColorRepeats

        include "tms.inc"
        include "z180.inc"
        include "cpm.inc"

OldSP:
        defw    0