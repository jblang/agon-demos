; Agon VDU commands equates and convenience functions 
; Copyright 2024 J.B. Langston
;
; Permission is hereby granted, free of charge, to any person obtaining a 
; copy of this software and associated documentation files (the "Software"), 
; to deal in the Software without restriction, including without limitation 
; the rights to use, copy, modify, merge, publilh, distribute, sublicense, 
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

; convenience macros
    macro SendByteA
        rst.lil $10
    endmacro

    macro SendByte value
        ld a, value
        rst.lil $10
    endmacro

    macro SendBytesHLBC
        rst.lil $18
    endmacro

    macro SendBytes addr, length
        ld hl, addr
        ld bc, length
        rst.lil $18
    endmacro

    macro SendString addr, term
        ld hl, addr
        ld bc, 0
        ld a, term
        rst.lil $18
    endmacro

; VDU commands

; https://agonconsole8.github.io/agon-docs/vdp/VDU-Commands/
VduNull: equ 0
VduPrinterChar: equ 1
VduPrinterOn: equ 2
VduPrinterOff: equ 3
VduTextCursor: equ 4
VduGraphicsCursor: equ 5
VduEnableScreen: equ 8
VduBeep: equ 7
VduCursorLeft: equ 8
VduCursorRight: equ 9
VduCursorDown: equ 10
VduCursorUp: equ 11
VduClearScreen: equ 12
VduCarriageReturn: equ 13
VduPageModeOn: equ 14
VduPageModeOff: equ 15
VduClearGraphics: equ 16
VduTextColor: equ 17
VduBackgroundColor: equ 128
VduGraphicsColor: equ 18
VduSetPalette: equ 19
VduResetPalette: equ 20
VduDisableScreen: equ 21
VduSetMode: equ 22
VduCustomChar: equ 23
VduCommand: equ 23
VduGraphicsViewport: equ 24
VduPlot: equ 25
VduResetViewports: equ 26
VduOutputChar: equ 27
VduTextViewport: equ 28
VduGraphicsOrigin: equ 29
VduCursorHome: equ 30
VduCursorMove: equ 31
VduBackspace: equ 127

; https://agonconsole8.github.io/agon-docs/vdp/System-Commands/
VduSystem: equ 0
VduVsyncBufferSwap: equ $C3

; https://agonconsole8.github.io/agon-docs/vdp/VDU-Commands/#vdu-23-1-n-cursor-control
VduCursorDisplay: equ 1
VduCursorHide: equ 0
VduCursorShow: equ 1
VduCursorSteady: equ 2
VduCursorFlash: equ 3

; https://agonconsole8.github.io/agon-docs/vdp/VDU-Commands/#vdu-23-16-setting-mask-define-cursor-movement-behaviour
VduCursorMovement: equ 16
VduScrollProtection: equ %00000001

; standard control characters
CR:	    equ     0dh
LF:	    equ     0ah
EOS:    equ     0       ; end of string (null terminated)

; output a character
; A = character to output
CharOut:
    rst.lil $10
    ret

; output a string terminated by EOS
; HL = address of string
StringOut:
    ld bc, 0
    ld a, EOS
    ; fallthrough

; output a specified number of bytes
; HL = address of bytes
; BC = byte count
BytesOut:
    SendBytesHLBC
    ret

; output a space
Space:
    SendByte ' '
    ret

; output a new line
NewLine:
    SendByte CR
    SendByte LF
    ret
    
; set the VDP mode
; C = mode number
VdpMode:
    SendByte VduSetMode
    SendByte c
    ret

; clear the screen
ClearScreen:
    SendByte VduClearScreen
    ret

; move to X, Y coordinates
; C = x coordinate
; B = Y coordinate
MoveTo:
    SendByte VduCursorMove
    SendByte c
    SendByte b
    ret

; set default text color
DefaultTextColor:
    ld b, 0                     ; white on black in default palette
    ld c, 15
    ; fallthrough

; set text color
; C = pen (foreground) color
; B = paper (background) color
TextColor:
    SendByte VduTextColor       ; pen (foreground)
    SendByte c
    SendByte VduTextColor       ; paper (background)
    ld a, b
    add VduBackgroundColor
    rst.lil $10
    ret

; hide the text cursor
TextCursorHide:
    SendByte VduCommand
    SendByte VduCursorDisplay
    SendByte VduCursorHide
    ret

; show the text cursor
TextCursorShow:
    SendByte VduCommand
    SendByte VduCursorDisplay
    SendByte VduCursorShow
    ret

; Set the specified cursor movement bits
; C = bits to set
SetCursorMovementBit:
    SendByte VduCommand
    SendByte VduCursorMovement
    ld a, c
    cpl
    SendByteA
    cpl
    SendByteA
    ret

; clear the specified cursor movement bits
; C = bits to clear
ClearCursorMovementBit:
    SendByte VduCommand
    SendByte VduCursorMovement
    ld a, c
    cpl
    SendByteA
    xor a
    SendByteA
    ret

; prevent the screen from scrolling when the last character is written
EnableScrollProtection:
    ld c, VduScrollProtection
    jp SetCursorMovementBit

; allow the screen to scroll when the last character is written
DisableScrollProtection:
    ld c, VduScrollProtection
    jp ClearCursorMovementBit
    ret

; wait for vsync (and swap buffers if using a buffered video mode)
WaitVSync:
    SendByte VduCommand
    SendByte VduSystem
    SendByte VduVsyncBufferSwap
    ret