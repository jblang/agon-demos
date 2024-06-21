; Agon VDU commands equates and convenience functions 
; Copyright 2024 J.B. Langston
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

; VDP MOS calls
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/MOS-API.md#rst-10h-output-a-single-character-to-the-vdp
VduSendByte: equ $10
VduSendBytes: equ $18

; Convenience macros
    macro SendByte value
        ld a, value
        rst.lis VduSendByte
    endmacro

    macro SendBuff addr, length
        ld hl, addr
        ld bc, length
        rst.lis VduSendBytes
    endmacro

    macro SendString addr, term
        ld hl, addr
        ld bc, 0
        ld a, term
        rst.lis VduSendBytes
    endmacro

; VDU commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md
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
VduSetPaletteRGB: equ $ff
VduResetPalette: equ 20
VduDisableScreen: equ 21
VduSetMode: equ 22
VduCustomChar: equ 23
VduCommand: equ 23
VduGraphicsViewport: equ 24
VduPlot: equ 25
VduResetViewports: equ 26
VduEscapeChar: equ 27
VduTextViewport: equ 28
VduGraphicsOrigin: equ 29
VduCursorHome: equ 30
VduCursorMove: equ 31
VduBackspace: equ 127

; Screen Modes
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Screen-Modes.md
VduMode640x480x16: equ 0
VduMode640x480x4: equ 1
VduMode640x480x2: equ 2
VduMode640x240x64: equ 3
VduMode640x240x16: equ 4
VduMode640x240x4: equ 5
VduMode640x240x2: equ 6
VduModeTeletext: equ 7
VduMode320x240x64: equ 8
VduMode320x240x16: equ 9
VduMode320x240x4: equ 10
VduMode320x240x2: equ 11
VduMode320x200x64: equ 12
VduMode320x200x16: equ 13
VduMode320x200x4: equ 14
VduMode320x200x2: equ 15
VduMode800x600x4: equ 16
VduMode800x600x2: equ 17
VduMode1024x768x2: equ 18

VduModeDoubleBuffered: equ 128
VduMode640x480x4DB: equ VduMode640x480x4 + VduModeDoubleBuffered
VduMode640x480x2DB: equ VduMode640x480x2 + VduModeDoubleBuffered
VduMode640x240x16DB: equ VduMode640x240x16 + VduModeDoubleBuffered
VduMode640x240x4DB: equ VduMode640x240x4 + VduModeDoubleBuffered
VduMode640x240x2DB: equ VduMode640x240x2 + VduModeDoubleBuffered
VduMode320x240x64DB: equ VduMode320x240x64 + VduModeDoubleBuffered
VduMode320x240x16DB: equ VduMode320x240x16 + VduModeDoubleBuffered
VduMode320x240x4DB: equ VduMode320x240x4 + VduModeDoubleBuffered
VduMode320x240x2DB: equ VduMode320x240x2 + VduModeDoubleBuffered
VduMode320x200x64DB: equ VduMode320x200x64 + VduModeDoubleBuffered
VduMode320x200x16DB: equ VduMode320x200x16 + VduModeDoubleBuffered
VduMode320x200x4DB: equ VduMode320x200x4 + VduModeDoubleBuffered
VduMode320x200x2DB: equ VduMode320x200x2 + VduModeDoubleBuffered

; Cursor display Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md#vdu-23-1-n-cursor-control
VduCursorDisplay: equ 1
VduCursorHide: equ 0
VduCursorShow: equ 1
VduCursorSteady: equ 2
VduCursorFlash: equ 3

; Cursor movement commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md#vdu-23-16-setting-mask-define-cursor-movement-behaviour
VduCursorMovement: equ 16
VduCursorWrap: equ %00010000
VduScrollProtection: equ %00000001

; System commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/System-Commands.md
VduSystemCommand: equ 0
VduResetChars: equ $91
VduMapCharToBitmap: equ $92
VduPrintBufferLiteral: equ $9B
VduVsyncBufferSwap: equ $C3

; Context management commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Context-Management-API.md
VduContextManagement: equ $C8
VduContextSelectStack: equ 0
VduContextDeleteStack: equ 1
VduContextReset: equ 2

VduContextResetGraphicsPainting: equ $01
VduContextResetGraphicsPositioning: equ $02
VduContextResetTextPainting: equ $04
VduContextResetTextCursorVisuals: equ $08
VduContextResetTextCursorBehavior: equ $10
VduContextResetFonts: equ $20
VduContextResetBitmapChars: equ $40
VduContextResetReserved: equ $80
VduContextResetAll: equ $ff

; Buffer commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Buffered-Commands-API.md
VduBufferCommand: equ $A0
VduBufferWriteBlock: equ 0
VduBufferCall: equ 1
VduBufferClear: equ 2
VduBufferCopyBlocks: equ 13
VduBufferBitmapExpand: equ 72

VduBufferBitmapExpandBppMask: equ 3
VduBufferBitmapExpandAlignBit: equ 3
VduBufferBitmapExpandMappingBufferBit: equ 4

VduBufferSpecial: equ $ffff

; Buffer Conditional Commands
VduBufferConditionalJump: equ 8

VduBufferConditionExists: equ 0
VduBufferConditionNotExists: equ 1
VduBufferConditionEqual: equ 2
VduBufferConditionNotEqual: equ 3
VduBufferConditionLessThan: equ 4
VduBufferConditionGreaterThan: equ 5
VduBufferConditionLessOrEqual: equ 6
VduBufferConditionGreaterOrEqual: equ 7
VduBufferConditionAnd: equ 8
VduBufferConditionOr: equ 9

; Buffer Adjust Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Buffered-Commands-API.md#command-5-adjust-buffer-contents
VduBufferAdjust: equ 5

VduBufferAdjustNot: equ 0
VduBufferAdjustNegate: equ 1
VduBufferAdjustSet: equ 2
VduBufferAdjustAdd: equ 3
VduBufferAdjustAddCarry: equ 4
VduBufferAdjustAnd: equ 5
VduBufferAdjustOr: equ 6
VduBufferAdjustXor: equ 7

VduBufferFlagAdvancedOffsets: equ $10
VduBufferFlagFetchOperand: equ $20
VduBufferFlagMultipleTarget: equ $40
VduBufferFlagMultipleOperand: equ $80

; Bitmap Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Bitmaps-API.md
VduBitmapCommand: equ 27
VduBitmapSelect: equ 0
VduBitmapLoad: equ 1
VduBitmapSolidColor: equ 2
VduBitmapDraw: equ 3
VduBitmapSelectBuffer: equ $20
VduBitmapCreateBuffer: equ $21
VduBitmapFormatRGBA8888: equ 0
VduBitmapFormatRGBA2222: equ 1
VduBitmapFormatMono: equ 2
VduBitmapFormatNative: equ 3

; Standard control characters
CR:	    equ     0dh
LF:	    equ     0ah
EOS:    equ     0       ; end of string (null terminated)

; Output a character
; Params:
;   A = character to output
CharOut:
    rst.lil VduSendByte
    ret

; output a string terminated by EOS
; Params:
;   HL = address of string
StringOut:
    push af
    push bc
    ld bc, 0
    ld a, EOS
    call BytesOut
    pop bc
    pop af
    ret

; Output a specified number of bytes
; Params:
;   HL = address of bytes
;   BC = byte count
BytesOut:
    rst.lis VduSendBytes
    ret

; Output a space
Space:
    push af
    SendByte ' '
    pop af
    ret

; Output a new line
NewLine:
    push af
    SendByte CR
    SendByte LF
    pop af
    ret
    
; Set the VDP mode
; Params:
;   C = mode number
VdpMode:
    push af
    SendByte VduSetMode
    SendByte c
    pop af
    ret

; Reset the VDP context
; Params:
;   C = flags
ResetContext:
    push af
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduContextManagement
    SendByte VduContextReset
    SendByte c
    pop af
    ret

VdpReset:
    call ResetBitmapChars
    ld de, VduBufferSpecial         ; clear all buffers
    call ClearBuffer
    ld c, VduContextResetAll
    call ResetContext
    ld c, VduMode640x480x16         ; switch to mode 0
    call VdpMode
    ret

; Clear the screen
ClearScreen:
    push af
    SendByte VduClearScreen
    pop af
    ret

CursorHome:
    push af
    SendByte VduCursorHome
    pop af
    ret

; Move to X, Y coordinates
; Params:
;   C = x coordinate
;   B = Y coordinate
MoveTo:
    push af
    SendByte VduCursorMove
    SendByte c
    SendByte b
    pop af
    ret

; Set default text color
DefaultTextColor:
    push bc
    ld b, 0                     ; white on black in default palette
    ld c, 15
    call TextColor
    pop bc
    ret

; Set text color
; Params:
;   C = foreground color
;   B = background color
TextColor:
    push af
    SendByte VduTextColor       ; pen (foreground)
    SendByte c
    SendByte VduTextColor       ; paper (background)
    ld a, b
    add VduBackgroundColor
    rst.lis VduSendByte
    pop af
    ret

; Hide the text cursor
TextCursorHide:
    push af
    SendByte VduCommand
    SendByte VduCursorDisplay
    SendByte VduCursorHide
    pop af
    ret

; Show the text cursor
TextCursorShow:
    push af
    SendByte VduCommand
    SendByte VduCursorDisplay
    SendByte VduCursorShow
    pop af
    ret

; Set the specified cursor movement bits
; Params:
;   C = bits to set
SetCursorMovementBit:
    push af
    SendByte VduCommand
    SendByte VduCursorMovement
    SendByte c
    ld a, c
    cpl
    SendByte a
    pop af
    ret

; Clear the specified cursor movement bits
; Params:
;   C = bits to clear
ClearCursorMovementBit:
    push af
    SendByte VduCommand
    SendByte VduCursorMovement
    SendByte 0
    ld a, c
    cpl
    SendByte a
    pop af
    ret

; Prevent the screen from scrolling when the last character is written
EnableScrollProtection:
    push bc
    ld c, VduScrollProtection
    call SetCursorMovementBit
    pop bc
    ret

; Allow the screen to scroll when the last character is written
DisableScrollProtection:
    push bc
    ld c, VduScrollProtection
    call ClearCursorMovementBit
    pop bc
    ret

; Wait for vsync (and swap buffers if using a buffered video mode)
WaitVSync:
VsyncBufferSwap:
    push af
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduVsyncBufferSwap
    pop af
    ret

; Round the accumulator to the top 2 most significant bits
Round2MSB:
    bit 5, a
    jp nz, Round2MSBUp
    and %11000000
    ret
Round2MSBUp:
    and %11000000
    add %01000000
    ret nc
    sub %01000000
    ret

; return the RGBA2222 equivalent of RGBA bytes
; Params:
;   HL = pointer to RGB888 format input buffer
;   DE = pointer to RGBA2222 format output buffer
;   B = number of colors to convert
; Return:
;   A = RGBA2222 value
MakeRGBA2222:
    push de
    ld a, (hl)
    inc hl
    call Round2MSB
    rrca
    rrca
    rrca
    rrca
    rrca
    rrca
    ld d, a
    ld a, (hl)
    inc hl
    call Round2MSB
    or a
    rrca
    rrca
    rrca
    rrca
    ld e, a
    ld a, (hl)
    inc hl
    call Round2MSB
    or a
    rrca
    rrca
    or d
    or e
    or $c0                  ; alpha
    pop de
    ld (de), a
    inc de
    djnz MakeRGBA2222
    ret

; Clear a buffer
; Params:
;   DE = buffer id
ClearBuffer:
    push af
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduBufferCommand
    SendByte e                  ; buffer id
    SendByte d
    SendByte VduBufferClear
    pop af
    ret

; Call a buffer
; Params:
;   DE = buffer id
CallBuffer:
    push af
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduBufferCommand
    SendByte e                  ; buffer id
    SendByte d
    SendByte VduBufferCall
    pop af
    ret

; Print buffer contents to screen literally without processing commands
; Params:
;   DE = buffer id
PrintBuffer:
    push af
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduPrintBufferLiteral
    SendByte e
    SendByte d
    pop af
    ret

ClearAndSendBuffer:
    call ClearBuffer
    ; fallthrough

; Send a block of data to a buffer
; Params:
;   DE = buffer id
;   BC = buffer length
;   HL = buffer data
SendBufferBlock:
    push af
    push hl
    push bc
    push de
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduBufferCommand
    SendByte e                  ; buffer id
    SendByte d
    SendByte VduBufferWriteBlock
    SendByte c                  ; length
    SendByte b
    rst.lis VduSendBytes
    pop de
    pop bc
    pop hl
    pop af
    ret

; Select a bitmap using buffer ID
; Params:
;   DE = buffer id
SelectBitmapBuffer:
    push af
    SendByte VduCommand
    SendByte VduBitmapCommand
    SendByte VduBitmapSelectBuffer
    SendByte e                  ; buffer id
    SendByte d
    pop af
    ret

; Create selected bitmap from buffer
; Params:
;   BC = width
;   DE = height
;   L = format
CreateBitmapFromBuffer:
    push af
    SendByte VduCommand
    SendByte VduBitmapCommand
    SendByte VduBitmapCreateBuffer
    SendByte c                  ; width
    SendByte b
    SendByte e                  ; height
    SendByte d
    SendByte l                  ; format
    pop af
    ret

; map character number to bitmap using buffer id
; C = char
; DE = bitmap id
MapCharToBitmap: 
    push af
    SendByte VduCommand
    SendByte VduSystemCommand
    SendByte VduMapCharToBitmap
    SendByte c                  ; character number
    SendByte e                  ; bitmap id
    SendByte d
    pop af
    ret

; Reset character to bitmap mappings
ResetBitmapChars:
    push bc
    push de
    ld c, 0
    ld de, $ffff
ResetBitmapCharLoop:
    call MapCharToBitmap
    inc c
    jp nz, ResetBitmapCharLoop
    pop de
    pop bc
    ret

; copy the characters in one buffer to another, escaping as needed
; Params:
;   HL = source buffer address
;   DE = destination buffer address
;   BC = buffer length
EscapeBuffer:
    push af
EscapeBufferLoop:
    ld a, (hl)                  ; check if current chracter needs escape
    cp ' '
    jp c, EscapeBufferYes
    cp 127
    jp nz, EscapeBufferNo
EscapeBufferYes:                ; yes, copy the escape character first
    ld a, VduEscapeChar
    ld (de), a
    inc de
EscapeBufferNo:                 ; no, just copy the character
    ldi
    jp pe, EscapeBufferLoop
    pop af
    ret

; send a character, escaped if necessary
; A = char to send
EscapeChar:
    cp ' '                      ; check if A contains a control char
    jp c, EscapeCharYes
    cp 127
    jp nz, EscapeCharNo
EscapeCharYes:                  ; if so, send escape char first
    push af
    SendByte VduEscapeChar
    pop af
EscapeCharNo:                   ; if not, just send char
    SendByte a
    ret

; Load a data table into the VDU
; IX = table address
; HL = entry prefix function (0 for none)
; D = entry stride
; E = entry count
; BC = free for use in prefix function
LoadTable:
    call TablePrefix
    push de
LoadTableLoop:
    ld a, (ix+0)
    SendByte a
    inc ix
    dec d
    jp nz, LoadTableLoop
    pop de
    dec e
    jp nz, LoadTable
    ret

; call table prefix function in HL, unless it is 0
; HL = function pointer
TablePrefix:
    ld a, h
    or l
    ret z
    jp (hl)