; Plasma Effect for Z80
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


; Plascii Petsma by Cruzer/Camelot has one of the nicest looking plasma effects I've seen for
; the C64. Since he included the source code, I was able to port it to the Z80 and TMS9918.   

; I have added the following interactive features of my own:
; - change the palette independent of the effect
; - hold a particular effect on screen indefinitely
; - switch immediately to a new effect
; - runtime generation of random effects
; - adjust parameters to customize an effect

; Before getting into a specific implementation, it helps to understand how plasma effects work
; in general. Rather than write another explanation when others have already done it well, I'll
; refer you to  this one, which covers the basic concepts using C code:
; https://lodev.org/cgtutor/plasma.html

NumSinePnts:    equ 8

; uncomment ONLY ONE of these depending on the platform
        include "agon.inc"
        ;include "rcmsx.inc"

Main:
        call    RandomSeed
        call    MakeSineTable
        call    MakeSpeedCode
        call    InitGraphics
        call    FirstEffect
        call    EnableInterrupt

MainLoop:
        ld      a, (HoldEffect)
        or      a
        jp      nz, NoEffectCycle
        ld      hl, DurationCnt
        dec     (hl)
        call    z, NextEffect
NoEffectCycle:
        ld      a, (StopAnimation)
        or      a
        call    z, CalcPlasmaFrame
        ;call    WaitVSync
        call    SendScreenBuffer
        ;call    GetKey
        ;jp      z, MainLoop
        ;call    ProcessCommand
        jp      MainLoop
        jp      Exit

About:
        defb    "Plasma for Z80 Computers", CR, LF
        defb    "Z80 Code by J.B. Langston", CR, LF, CR, LF
        defb    "Color Palettes and Sine Routines ported from "
        defb    "Plascii Petsma by Cruzer/Camelot", CR, LF
        defb    "Gradient Patterns ripped from "
        defb    "Produkthandler Kom Her by Cruzer/Camelot", CR, LF, CR, LF
        defb    "Press 'q' to quit, '?' for help.", CR, LF, EOS

Help:
        defb    CR, LF, "Commands:", CR, LF
        defb    " ?     help", CR, LF
        defb    " q     quit", CR, LF
        defb    " h     hold current effect on/off", CR, LF
        defb    " p     switch palette", CR, LF
        defb    " n     next effect", CR, LF
        defb    " d     default values", CR, LF
        defb    " a     animation on/off", CR, LF
        defb    " r     toggle random/playlist", CR, LF
        defb    " v     view parameters", CR, LF, CR, LF
        defb    "Parameter Selection:", CR, LF
        defb    " x     x increments", CR, LF
        defb    " y     y increments", CR, LF
        defb    " i     initial values", CR, LF
        defb    " c     linear animation speed", CR, LF
        defb    " s     sine animation speeds", CR, LF
        defb    " f     distortion frequencies", CR, LF, CR, LF
        defb    "Parameter Modification:", CR, LF
        defb    " 1-8   increment selected parameter (+ shift to decrement)", CR, LF
        defb    " 0     clear selected parameters", CR, LF, EOS

; command keys grouped by function
Commands:
        defb    "?qhpndavr0"
ModeSelectCommands:
        defb    "xyisfc"
IncDecCommands:
        defb    "12345678"
NumIncCommands: equ $ - IncDecCommands
        defb    "!@#$%^&*"
NumCommands:    equ $ - Commands

; pointers to command functions; must be 1-1 correspondence to commands
CommandPointers:
        DefPointer    ShowHelp
        DefPointer    Exit
        DefPointer    ToggleHold
        DefPointer    NextPalette
        DefPointer    NextEffect
        DefPointer    InitEffect
        DefPointer    ToggleAnimation
        DefPointer    ViewParameters
        DefPointer    ToggleRandomParams
        DefPointer    ClearParameters

; pointers to parameters; must be 1-1 correspondence to mode select commands
ParameterPointers:
        DefPointer    SineAddsX
        DefPointer    SineAddsY
        DefPointer    SineStartsY
        DefPointer    SineSpeeds
        DefPointer    PlasmaFreqs
        DefPointer    CycleSpeed
        DefPointer    CycleSpeed+1

; dispatch command key in a
ProcessCommand:
        ld      hl, Commands
        ld      b, NumCommands
CheckCommandLoop:
        cp      (hl)
        jp      z, FoundCommandKey
        inc     hl
        djnz    CheckCommandLoop
        ret

; determine what category of command it is and handle appropriately
FoundCommandKey:
        ld      de, IncDecCommands
        or      a
        sbc     hl, de
        jp      nc, FoundIncDecCommand
        add     hl, de
        ld      de, ModeSelectCommands
        or      a
        sbc     hl, de
        jp      nc, FoundModeSelectCommand
        add     hl, de
        ld      de, Commands
        or      a
        sbc     hl, de
        ld      de, CommandPointers
        add     hl, hl
        add     hl, de
        ld      a, (hl)
        inc     hl
        ld      h, (hl)
        ld      l, a
        jp      (hl)

; mode select command; set pointers to appropriate mode variables
SelectedParameter:
        DefPointer    CycleSpeed
SelectedParameterLength:
        defb    1

FoundModeSelectCommand:
        ld      de, ParameterPointers
        add     hl, hl
        add     hl, de
        ld      e, (hl)
        inc     hl
        ld      d, (hl)
        ld      (SelectedParameter), de
        inc     hl
        ld      a, (hl)
        inc     hl
        ld      h, (hl)
        ld      l, a
        or      a
        sbc     hl, de
        ld      a, l
        ld      (SelectedParameterLength), a
        ret

; increment/decrement command; adjust selected variable
FoundIncDecCommand:
        ld      a, l
        cp      NumIncCommands
        push    af
        and     7
        ld      l, a
        push    hl
        ld      hl, SelectedParameterLength
        cp      (hl)
        pop     hl
        jp      nc, AbortChangeParameter
        ld      de, (SelectedParameter)
        add     hl, de
        pop     af
        call    c, IncParameter
        call    nc, DecParameter
        ld      de, SineSpeeds
        or      a
        sbc     hl, de
        call    c, CalcPlasmaStarts
        ret
AbortChangeParameter:
        pop     af
        ret
IncParameter:
        inc     (hl)
        ret
DecParameter:
        dec     (hl)
        ret

; feature toggles
HoldEffect:
        defb    0
StopAnimation:
        defb    0
UseRandomParams:
        defb    0

ToggleRandomParams:
        ld      a, (UseRandomParams)
        xor     $ff
        ld      (UseRandomParams), a
        ret

ToggleAnimation:
        ld      a, (StopAnimation)
        xor     $ff
        ld      (StopAnimation), a
        jp      UpdateScreen

ToggleHold:
        ld      a, (HoldEffect)
        xor     $ff
        ld      (HoldEffect), a
        ret

; reset selected parameter values to 0
ClearParameters:
        ld      hl, (SelectedParameter)
        ld      a, (SelectedParameterLength)
        ld      b, a
        xor     a
ClearParameterLoop:
        ld      (hl), a
        inc     hl
        djnz    ClearParameterLoop
        jp      CalcPlasmaStarts

; parameter display names
SineAddsXMsg:
        defb CR, LF, "x increment: $"
SineAddsYMsg:
        defb CR, LF, "y increment: $"
SineStartsMsg:
        defb CR, LF, "init values: $"
SineSpeedsMsg:
        defb CR, LF, "sine speeds: $"
PlasmaFreqMsg:
        defb CR, LF, "plasma freq: $"
CycleSpeedMsg:
        defb CR, LF, "cycle speed: $"

; display current parameter values
ViewParameters:
        ld      hl, PlasmaParams
        ld      de, SineAddsXMsg
        call    ShowSinePnts
        ld      de, SineAddsYMsg
        call    ShowSinePnts
        ld      de, SineStartsMsg
        call    ShowSinePnts
        ld      de, SineSpeedsMsg
        call    ShowTwoParams
        ld      de, PlasmaFreqMsg
        call    ShowTwoParams
        ld      de, CycleSpeedMsg
        call    ShowOneParam
        call    NewLine
        ret
ShowOneParam:
        ld      b, 1
        jp      ShowBParams
ShowTwoParams:
        ld      b, 2
        jp      ShowBParams
ShowSinePnts:
        ld      b, NumSinePnts
ShowBParams:
        push    hl
        push    bc
        call    StringOut
        pop     bc
        pop     hl
ShowParameterLoop:
        ld      a, (hl)
        inc     hl
        push    hl
        push    bc
        call    HexOut
        call    Space
        pop     bc
        pop     hl
        djnz    ShowParameterLoop
        ret

        include "sine.inc"
        include "random.inc"
        include "utility.inc"

; RandomParameters generates a complete set of random parameters
RandomParameters:
        ld      d, 0
        ld      c, 7                    ; -8 to 7
        ld      b, NumSinePnts
        ld      hl, SineAddsX
        call    RandomSeries
        ld      c, 3                    ; -4 to 3
        ld      b, NumSinePnts
        ld      hl, SineAddsY
        call    RandomSeries
        ld      c, $7f                  ; -128 to 127
        ld      b, NumSinePnts
        ld      hl, SineStartsY
        call    RandomSeries
        ld      c, 3                    ; -4 to 3
        ld      b, 2
        ld      hl, SineSpeeds
        call    RandomSeries
        ld      c, 3                    ; 1 to 8
        ld      d, 5
        ld      b, 2
        ld      hl, PlasmaFreqs
        call    RandomSeries
        ld      c, 7                    ; -16 to -1
        ld      d, -8
        ld      b, 1
        ld      hl, CycleSpeed
        call    RandomSeries
        call    RandomNumber            ; randomly select palette
        ld      a, l
        and     $f                      ; assumes 16 palettes of 8 colors each
        ld      h, 0
        ld      l, a
        add     hl, hl
        add     hl, hl
        add     hl, hl
        ld      de, ColorPalettes
        add     hl, de
        ld      (ColorPalette), hl
        call    CalcPlasmaStarts
        jp      LoadPalette
        
; select and initialize plasma effects
DurationCnt:
        defb    0
PlasmaParamPnt:
        DefPointer    0

NextEffect:
        ld      a, (UseRandomParams)
        or      a
        jp      nz, RandomParameters
        ld      hl, (PlasmaParamPnt)
        ld      de, PlasmaParamLen
        add     hl, de
        ld      (PlasmaParamPnt), hl
        ld      de, LastPlasmaParam
        or      a
        sbc     hl, de
        jp      c, InitEffect
        ; fallthrough

FirstEffect:
        ld      hl, PlasmaParamList
        ld      (PlasmaParamPnt), hl
        ; fallthrough

InitEffect:
        ld      hl, (PlasmaParamPnt)            ; copy parameters
        ld      de, PlasmaParams
        ld      bc, PlasmaParamLen
        ldir
        
        xor     a                               ; reset counters
        ld      (PlasmaCnts), a
        ld      (PlasmaCnts+1), a
        ld      (CycleCnt), a
        ld      (DurationCnt), a
        
        call    CalcPlasmaStarts
        call    LoadPalette
        ret

; PlasmaParams holds parameters for the current effect
PlasmaParams:
SineAddsX:
        defs    NumSinePnts
SineAddsY:
        defs    NumSinePnts
SineStartsY:
        defs    NumSinePnts
SineSpeeds:
        defs    2
PlasmaFreqs:
        defs    2
CycleSpeed:
        defs    1
ColorPalette:
        DefPointer    0
PlasmaParamLen: equ $ - PlasmaParams

; PlasmaParamList contains pre-defined plasma parameters
PlasmaParamList:
        defb    $fa,$05,$03,$fa,$07,$04,$fe,$fe
        defb    $fe,$01,$fe,$02,$03,$ff,$02,$02
        defb    $5e,$e8,$eb,$32,$69,$4f,$0a,$41
        defb    $fe,$fc
        defb    $06,$07
        defb    $ff
        DefPointer Pal01

        defb    $04,$05,$fc,$02,$fc,$03,$02,$01
        defb    $00,$01,$03,$fd,$02,$fd,$fe,$00
        defb    $51,$a1,$55,$c1,$0d,$5a,$dd,$26
        defb    $fe,$fd
        defb    $08,$08
        defb    $f8
        DefPointer Pal06

        defb    $f9,$06,$fe,$fa,$fa,$00,$07,$fb
        defb    $02,$01,$02,$03,$03,$00,$fd,$00
        defb    $34,$85,$a6,$11,$89,$2b,$fa,$9c
        defb    $fc,$fb
        defb    $09,$08
        defb    $fa
        DefPointer Pal09

        defb    $00,$01,$03,$00,$01,$ff,$04,$fc
        defb    $01,$ff,$03,$fe,$fe,$03,$02,$02
        defb    $f3,$02,$0b,$89,$8c,$d3,$23,$aa
        defb    $fe,$01
        defb    $07,$07
        defb    $08
        DefPointer Pal0a

        defb    $04,$04,$04,$fc,$fd,$04,$ff,$fc
        defb    $01,$02,$02,$01,$ff,$00,$ff,$01
        defb    $3a,$21,$53,$93,$39,$b7,$26,$99
        defb    $fd,$fe
        defb    $05,$06
        defb    $03
        DefPointer Pal04

        defb    $fd,$fd,$fd,$02,$04,$00,$fd,$02
        defb    $03,$02,$fd,$02,$03,$fe,$ff,$ff
        defb    $bc,$99,$5d,$2f,$e6,$16,$af,$0e
        defb    $fd,$ff
        defb    $07,$07
        defb    $f5
        DefPointer Pal07

        defb    $fc,$00,$00,$ff,$04,$04,$00,$01
        defb    $fd,$03,$00,$02,$00,$03,$02,$03
        defb    $30,$c7,$07,$60,$36,$2b,$e8,$ec
        defb    $ff,$fe
        defb    $09,$03
        defb    $f8
        DefPointer Pal05

        defb    $fd,$fc,$fe,$00,$00,$04,$fe,$01
        defb    $03,$03,$fe,$02,$00,$03,$fe,$00
        defb    $21,$d7,$34,$1b,$5d,$eb,$8e,$7d
        defb    $fd,$ff
        defb    $0a,$03
        defb    $fd
        DefPointer Pal03

        defb    $fe,$00,$ff,$01,$04,$02,$fe,$fd
        defb    $02,$01,$fe,$01,$03,$ff,$03,$ff
        defb    $0b,$0f,$ea,$8c,$e0,$f8,$05,$0e
        defb    $fc,$fd
        defb    $07,$06
        defb    $f8
        DefPointer Pal0c

        defb    $33,$04,$34,$fc,$dd,$24,$cf,$7c
        defb    $c1,$73,$02,$31,$fe,$a0,$ee,$01
        defb    $3a,$21,$53,$93,$39,$b7,$26,$99
        defb    $00,$00
        defb    $04,$01
        defb    $fd
        DefPointer Pal00

        defb    $ff,$00,$01,$ff,$02,$fe,$00,$02
        defb    $ff,$02,$01,$02,$fe,$01,$00,$00
        defb    $1d,$bb,$c5,$a3,$ab,$6c,$ed,$a6
        defb    $fd,$fe
        defb    $03,$03
        defb    $f8
        DefPointer Pal08

        defb    $02,$03,$fd,$fd,$01,$fc,$fd,$00
        defb    $01,$03,$fd,$fe,$fe,$03,$00,$00
        defb    $69,$ac,$3b,$c1,$fe,$21,$37,$84
        defb    $fc,$fd
        defb    $06,$05
        defb    $fa
        DefPointer Pal0b
LastPlasmaParam:

; NextPalette changes to the next color palette
NextPalette:
        ld      hl, (ColorPalette)
        ld      de, ColorPaletteLength
        add     hl, de
        ld      (ColorPalette), hl
        ld      de, ColorPaletteEnd
        or      a
        sbc     hl, de
        jp      c, LoadPalette
        ld      hl, ColorPalettes
        ld      (ColorPalette), hl
        ; fallthrough


; CalcPlasmaStarts calculates the initial value for each tile by summing together 8 sine waves of
; varying frequencies which combine to create the contours of a still image. Each sine wave is
; defined by a StartAngle, RowFreq and ColFreq which are applied to each X, Y coordinate as:
; StillFrame(x,y) = sum[n=1..8]: sin(StartAngle[n] + ColFreq[n] * x + RowFreq[n] * y)

; The calculation of the input angle for each X and Y coordinate is accomplished by successive
; additions of the RowFreq and ColFreq values for to the respective RowAngle and ColAngle
; accumulators.
CalcPlasmaStarts:
        ld      hl, SineStartsY         ; for each of 8 sine waves,
        ld      de, SinePntsY           ; initialize SinePntsY to SineStartsY
        ld      bc, NumSinePnts
        ldir
        ld      hl, PlasmaStarts
        ld      c, ScreenHeight         ; for each row...
YLoop:
        exx
        ld      bc, SinePntsY
        ld      hl, SineAddsY
        ld      de, SinePntsX
        exx
        ld      d, NumSinePnts
SinePntsYLoop:
        exx                             ; for each sine wave...
        ld      a, (bc)
        add     a, (hl)                 ; add SineAddsY to SinePntsY
        ld      (bc), a
        ld      (de), a                 ; initialize SinePntsX to SinePntsY
        inc     bc
        inc     de
        inc     hl
        exx
        dec     d
        jp      nz, SinePntsYLoop       ; ... next sine wave
        ld      b, ScreenWidth          ; for each column...
XLoop:
        exx
        ld      de, SinePntsX
        ld      hl, SineAddsX
        ld      b, NumSinePnts          ; for each sine wave...
SinePntsXLoop:
        ld      a, (de)
        add     a, (hl)                 ; add SineAddsX to SinePntsX
        ld      (de), a
        inc     de
        inc     hl
        djnz    SinePntsXLoop           ; ... next sine wave

        ld      hl, SineTable
        ld      de, SinePntsX
        xor     a                       ; initialize to zero
        ld      b, NumSinePnts          ; for each sine wave...
SineAddLoop:
        ex      af, af'
        ld      a, (de)                 ; look up SinePntsX in SineTable
        ld      l, a
        ex      af, af'
        add     a, (hl)                 ; accumulate values from SineTable
        inc     de
        djnz    SineAddLoop             ; ...next sine wave
        exx
        ld      (hl), a                 ; save accumulated value in PlasmaStarts
        inc     hl
        djnz    XLoop                   ; ... next column
        dec     c
        jp      nz, YLoop               ; ... next row
UpdateScreen:
        ld      hl, PlasmaStarts        ; transfer PlasmaStarts to screen buffer
        ld      de, ScreenBuffer
        ld      bc, ScreenSize
        ldir
        ret

SinePntsX:
        defs    NumSinePnts
SinePntsY:
        defs    NumSinePnts

; CalcPlasmaFrame applies distortion and color cycling effects to the original image StillFrame.  
;
; For each frame, tiles are shifted based on LinearSpeed and two SineSpeeds.  In addition, each 
; row is warped by sine waves defined by two RowWarp parameters. For each row y of frame f, the
; total offset applied to each tile of StillFrame is calcualted according to this formula:
; D(f,y) = LinearSpeed * f + (sum [n=0..1]: sin(SineSpeed[n] * f + RowWarp[n] * y)) / 2
CalcPlasmaFrame:
        ld      bc, PlasmaCnts
        ld      de, (SineSpeeds)        
        ld      a, (bc)                 
        ld      h, a                    
        add     a, e                    
        ld      (bc), a
        inc     bc
        ld      a, (bc)
        ld      l, a
        add     a, d
        ld      (bc), a                   
        ld      de, SineTable
        ld      e, h                    
        ld      h, d                    
        ld      bc, (PlasmaFreqs)       
        exx
        ld      de, CycleCnt
        ld      a, (de)
        ld      c, a
        ld      hl, CycleSpeed
        add     a, (hl)
        ld      (de), a                 
        ld      hl, PlasmaStarts
        ld      de, ScreenBuffer
        jp      SpeedCode

; calculate new plasma frame from starting point and current counts
PlasmaCnts:
        defw    0
CycleCnt:
        defb    0

; setup for speedcode:
;       de  = pointer to first sine table entry
;       hl  = pointer to second sine table entry
;       c   = amount to increment first sine pointer between lines
;       b   = amount to increment second sine pointer between lines
;       c'  = current cycle count
;       b'  = offset to add to starting value for current row
;       hl' = pointer to starting plasma values
;       de' = pointer to screen back buffer
;       a   = temporary calculations

RowSrc:
        exx
        ld      a, e
        add     a, c
        ld      e, a
        ld      a, l
        add     a, b
        ld      l, a
        ld      a, (de)
        add     a, (hl)
        rra
        exx
        adc     a, c
        ld      b, a
RowSrcLen:     equ $ - RowSrc

ColSrc:
        ld      a, (hl)
        add     a, b
        ld      (de), a
        inc     hl
        inc     de
ColSrcLen:      equ $ - ColSrc

; build unrolled loops for speed
MakeSpeedCode:
        ld      de, SpeedCode
        ld      a, ScreenHeight
RowLoop:
        ld      hl, RowSrc
        ld      bc, RowSrcLen
        ldir
        ex      af, af'
        ld      a, ScreenWidth
ColLoop:
        ld      hl, ColSrc
        ld      bc, ColSrcLen
        ldir
        dec     a
        jp      nz, ColLoop
        ex      af, af'
        dec     a
        jp      nz, RowLoop
        ld      a, (RetSrc)
        ld      (de), a
RetSrc:
        ret

NextPage:       equ $ + $ff
SineTable:      equ NextPage & PageMask         ; page align
Stack:          equ SineTable + $200
PlasmaStarts:   equ Stack
ScreenBuffer:   equ PlasmaStarts + ScreenSize
SpeedCode:      equ ScreenBuffer + ScreenSize