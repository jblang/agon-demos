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

; =============================================================================

; Ported from Plascii Petsma for C64 by Camelot: https://csdb.dk/release/?id=159933

; Plascii Petsma by Cruzer/Camelot has one of the nicest looking plasma effects I've seen for
; the C64. Since he included the source code, I was able to port it to the Z80 and TMS9918.   

; I have added the following interactive features of my own:
; - change the palette independent of the effect
; - hold a particular effect on screen indefinitely
; - switch immediately to a new effect
; - runtime generation of random effects
; - adjust parameters to customize an effect

; Before diving into this code, it helps to understand how plasma effects work in general.
; Rather than write another explanation when others have already done it well, I'll refer
; you to this one: https://lodev.org/cgtutor/plasma.html.

; =============================================================================
; uncomment ONLY ONE of these includes depending on the platform:

        include "agon.inc"
        ;include "rcmsx.inc"

; =============================================================================
; general includes

        include "utility.inc"
        include "data.inc"

; =============================================================================
; constants

NumSinePnts:    equ 8
NumSineSpeeds:  equ 2
NumPlasmaFreqs: equ 2
NumCycleSpeeds: equ 1

; =============================================================================
; central dispatch

Main:                                   ; initialization tasks
        call    RandomSeed
        call    MakeSineTable
        call    MakeSpeedCode
        call    InitGraphics
        call    FirstEffect
        call    EnableInterrupt

MainLoop:                               ; repetitive tasks
        ld      a, (HoldEffect)
        or      a
        jp      nz, NoEffectCycle
        ld      hl, DurationCnt
        dec     (hl)
        call    z, NextEffect
NoEffectCycle:
        call    CalcPlasmaFrame
        call    WaitVSync
        call    SendScreenBuffer
        call    GetKey
        JumpIf  'q', Exit               ; must be handled from main
        call    Dispatch
        jp      MainLoop

Dispatch:
        ; did you remember to add your new command to the help below?
        JumpIf  '?', ShowHelp
        JumpIf  'p', NextPalette
        JumpIf  'h', ToggleHold
        JumpIf  'n', NextEffect
        JumpIf  'd', InitEffect
        JumpIf  'a', ToggleAnimation
        JumpIf  'r', ToggleRandomParams
        JumpIf  'v', ViewParameters
        JumpIf  'x', SelectSineAddsX
        JumpIf  'y', SelectSineAddsY
        JumpIf  'i', SelectSineStartsY
        JumpIf  'c', SelectSineSpeeds
        JumpIf  's', SelectPlasmaFreqs
        JumpIf  'f', SelectCycleSpeed 
        JumpIf  '0', ClearParameters
        JumpIf  '>', IncSelected
        JumpIf  '<', DecSelected
        ret

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

About:
        defb    "Plasma for TMS9918", CR, LF
        defb    "Z80 Code by J.B. Langston", CR, LF, CR, LF
        defb    "Color Palettes and Sine Routines ported from "
        defb    "Plascii Petsma by Cruzer/Camelot", CR, LF
        defb    "Gradient Patterns ripped from "
        defb    "Produkthandler Kom Her by Cruzer/Camelot", CR, LF, CR, LF
        defb    "Press 'q' to quit, '?' for help.", CR, LF, EOS

; =============================================================================
; parameter configuration

SelectedParameter:
        DefPointer SineAddsX
SelectedParameterLength:
        defb    NumSinePnts
SelectedParameterNumber:
        defb    0

SelectParameterNumber:
        ; for numbers 1-8 set the parameter number
        sub     '1'
        ret     c
        ld      hl, SelectedParameterNumber
        cp      (hl)
        ret     nc
        ld      (SelectedParameterNumber), a
InvalidParameterNumber:
        add     '1'
        ret

        macro SelectParam addr, length
        ld      hl, addr
        ld      a, length
        jp      SelectGeneric
        endmacro

SelectSineAddsX:
        SelectParam SineAddsX, NumSinePnts
SelectSineAddsY:
        SelectParam SineAddsY, NumSinePnts
SelectSineStartsY:
        SelectParam SineStartsY, NumSinePnts
SelectSineSpeeds:
        SelectParam SineSpeeds, NumSineSpeeds
SelectPlasmaFreqs:
        SelectParam PlasmaFreqs, NumPlasmaFreqs
SelectCycleSpeed:
        SelectParam CycleSpeed, NumCycleSpeeds

SelectGeneric:
        ld      (SelectedParameter), hl
        ld      (SelectedParameterLength), a
        ret

GetSelected:
        ld      hl, (SelectedParameter)
        ld      a, (SelectedParameterNumber)
        ld      de, 0
        ld      e, a
        add     hl, de
        ret

IncSelected:
        call    GetSelected
        inc     (hl)
        ret

DecSelected:
        call    GetSelected
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

SineAddsXMsg:
        defb CR, LF, "x increment: ", EOS
SineAddsYMsg:
        defb CR, LF, "y increment: ", EOS
SineStartsMsg:
        defb CR, LF, "init values: ", EOS
SineSpeedsMsg:
        defb CR, LF, "sine speeds: ", EOS
PlasmaFreqMsg:
        defb CR, LF, "plasma freq: ", EOS
CycleSpeedMsg:
        defb CR, LF, "cycle speed: ", EOS

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
        ld      (ColorPalette), a
        call    CalcPlasmaStarts
        jp      LoadPalette
        
; =============================================================================
; effect intialization

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

; =============================================================================
; plasma calculations

; PlasmaParams holds parameters for the current effect
PlasmaParams:
SineAddsX:
        defs    NumSinePnts
SineAddsY:
        defs    NumSinePnts
SineStartsY:
        defs    NumSinePnts
SineSpeeds:
        defs    NumSineSpeeds
PlasmaFreqs:
        defs    NumPlasmaFreqs
CycleSpeed:
        defs    NumCycleSpeeds
ColorPalette:
        defb    0
PlasmaParamLen: equ $ - PlasmaParams

; NextPalette changes to the next color palette
NextPalette:
        ld      hl, ColorPalette        ; increment the palette number
        inc     (hl)
        ld      a, (hl)
        cp      ColorPaletteCount       ; check if it's beyond the last one
        jp      c, LoadPalette          ; if not, load it
        xor     a                       ; if so, wrap around to the first one
        ld      (hl), a
        ; fallthrough

; Load the currently selected color palette
LoadPalette:
        ld      hl, ColorPalettes       ; calculate address of selected palette
        ld      a, (ColorPalette)
        or      a
        jp      z, LoadColorTable
        ld      de, ColorPaletteStride
LoadPaletteLoop:
        add     hl, de
        dec     a
        jp      nz, LoadPaletteLoop
        jp      LoadColorTable


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
        ld      a, (StopAnimation)
        or      a
        ret     nz
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


; MakeSineTable builds the sine table for a complete period from a precalculated quarter period.
; The first 64 values are copied verbatim from the precomputed values. The next 64 values are
; flipped horizontally by copying them in reverse order. The last 128 values are flipped 
; vertically by complementing them. The vertically flipped values are written twice, first in
; forward order, and then in reverse order to flip them horizontally and complete the period.
; The resulting lookup table is 256 bytes long and stored on a 256-byte boundary so that a sine
; value can be looked up by loading a single register with the input value.

MakeSineTable:
        ld      bc, SineSrc             ; source values
        ld      de, SineTable           ; start of 1st quarter
        ld      hl, SineTable+$7f       ; end of 2nd quarter
        exx
        ld      b, $40                  ; counter
        ld      de, SineTable+$80       ; start of 3rd quarter
        ld      hl, SineTable+$ff       ; end of 4th quarter
SineLoop:
        exx
        ld      a, (bc)                 ; load source value
        inc     bc
        ld      (de), a                 ; store 1st quarter
        inc     de
        ld      (hl), a                 ; store 2nd quarter
        dec     hl                      ; in reverse order
        exx
        cpl                             ; flip vertically
        ld      (de), a                 ; store 3rd quarter
        inc     de
        ld      (hl), a                 ; store 4th quarter
        dec     hl                      ; in reverse order
        djnz    SineLoop
        ret

NextPage:       equ $ + $ff
SineTable:      equ NextPage & PageMask         ; page align
Stack:          equ SineTable + $200
PlasmaStarts:   equ Stack
ScreenBuffer:   equ PlasmaStarts + ScreenSize
SpeedCode:      equ ScreenBuffer + ScreenSize