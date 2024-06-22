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
VDU_SendByte: equ $10
VDU_SendBytes: equ $18

; Convenience macros
    macro SendByte value
        ld a, value
        rst.lis VDU_SendByte
    endmacro

    macro SendBuff addr, length
        ld hl, addr
        ld bc, length
        rst.lis VDU_SendBytes
    endmacro

    macro SendString addr, term
        ld hl, addr
        ld bc, 0
        ld a, term
        rst.lis VDU_SendBytes
    endmacro

; VDU commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md
VDU_Null: equ 0
VDU_PrinterChar: equ 1
VDU_PrinterOn: equ 2
VDU_PrinterOff: equ 3
VDU_TextCursor: equ 4
VDU_GraphicsCursor: equ 5
VDU_EnableScreen: equ 8
VDU_Beep: equ 7
VDU_CursorLeft: equ 8
VDU_CursorRight: equ 9
VDU_CursorDown: equ 10
VDU_CursorUp: equ 11
VDU_ClearScreen: equ 12
VDU_CarriageReturn: equ 13
VDU_PageModeOn: equ 14
VDU_PageModeOff: equ 15
VDU_ClearGraphics: equ 16
VDU_TextColor: equ 17
VDU_BackgroundColor: equ 128
VDU_GraphicsColor: equ 18
VDU_SetPalette: equ 19
VDU_SetPaletteRGB: equ $ff
VDU_ResetPalette: equ 20
VDU_DisableScreen: equ 21
VDU_SetMode: equ 22
VDU_CustomChar: equ 23
VDU_Command: equ 23
VDU_GraphicsViewport: equ 24
VDU_Plot: equ 25
VDU_ResetViewports: equ 26
VDU_EscapeChar: equ 27
VDU_TextViewport: equ 28
VDU_GraphicsOrigin: equ 29
VDU_CursorHome: equ 30
VDU_CursorMove: equ 31
VDU_Backspace: equ 127

; Screen Modes
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Screen-Modes.md
VDU_Mode640x480x16: equ 0
VDU_Mode640x480x4: equ 1
VDU_Mode640x480x2: equ 2
VDU_Mode640x240x64: equ 3
VDU_Mode640x240x16: equ 4
VDU_Mode640x240x4: equ 5
VDU_Mode640x240x2: equ 6
VDU_ModeTeletext: equ 7
VDU_Mode320x240x64: equ 8
VDU_Mode320x240x16: equ 9
VDU_Mode320x240x4: equ 10
VDU_Mode320x240x2: equ 11
VDU_Mode320x200x64: equ 12
VDU_Mode320x200x16: equ 13
VDU_Mode320x200x4: equ 14
VDU_Mode320x200x2: equ 15
VDU_Mode800x600x4: equ 16
VDU_Mode800x600x2: equ 17
VDU_Mode1024x768x2: equ 18

VDU_ModeDoubleBuffered: equ 128
VDU_Mode640x480x4DB: equ VDU_Mode640x480x4 + VDU_ModeDoubleBuffered
VDU_Mode640x480x2DB: equ VDU_Mode640x480x2 + VDU_ModeDoubleBuffered
VDU_Mode640x240x16DB: equ VDU_Mode640x240x16 + VDU_ModeDoubleBuffered
VDU_Mode640x240x4DB: equ VDU_Mode640x240x4 + VDU_ModeDoubleBuffered
VDU_Mode640x240x2DB: equ VDU_Mode640x240x2 + VDU_ModeDoubleBuffered
VDU_Mode320x240x64DB: equ VDU_Mode320x240x64 + VDU_ModeDoubleBuffered
VDU_Mode320x240x16DB: equ VDU_Mode320x240x16 + VDU_ModeDoubleBuffered
VDU_Mode320x240x4DB: equ VDU_Mode320x240x4 + VDU_ModeDoubleBuffered
VDU_Mode320x240x2DB: equ VDU_Mode320x240x2 + VDU_ModeDoubleBuffered
VDU_Mode320x200x64DB: equ VDU_Mode320x200x64 + VDU_ModeDoubleBuffered
VDU_Mode320x200x16DB: equ VDU_Mode320x200x16 + VDU_ModeDoubleBuffered
VDU_Mode320x200x4DB: equ VDU_Mode320x200x4 + VDU_ModeDoubleBuffered
VDU_Mode320x200x2DB: equ VDU_Mode320x200x2 + VDU_ModeDoubleBuffered

; Cursor display Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md#vdu-23-1-n-cursor-control
VDU_CursorDisplay: equ 1
VDU_CursorHide: equ 0
VDU_CursorShow: equ 1
VDU_CursorSteady: equ 2
VDU_CursorFlash: equ 3

; Cursor movement commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md#vdu-23-16-setting-mask-define-cursor-movement-behaviour
VDU_CursorMovement: equ 16
VDU_CursorWrap: equ %00010000
VDU_ScrollProtection: equ %00000001

; System commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/System-Commands.md
VDU_SystemCommand: equ 0
VDU_ResetChars: equ $91
VDU_MapCharToBitmap: equ $92
VDU_PrintBufferLiteral: equ $9B
VDU_VsyncBufferSwap: equ $C3

; Context management commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Context-Management-API.md
VDU_ContextManagement: equ $C8
VDU_ContextSelectStack: equ 0
VDU_ContextDeleteStack: equ 1
VDU_ContextReset: equ 2

VDU_ContextResetGraphicsPainting: equ $01
VDU_ContextResetGraphicsPositioning: equ $02
VDU_ContextResetTextPainting: equ $04
VDU_ContextResetTextCursorVisuals: equ $08
VDU_ContextResetTextCursorBehavior: equ $10
VDU_ContextResetFonts: equ $20
VDU_ContextResetBitmapChars: equ $40
VDU_ContextResetReserved: equ $80
VDU_ContextResetAll: equ $ff

; Buffer commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Buffered-Commands-API.md
VDU_BufferCommand: equ $A0
VDU_BufferWriteBlock: equ 0
VDU_BufferCall: equ 1
VDU_BufferClear: equ 2
VDU_BufferCopyBlocks: equ 13
VDU_BufferBitmapExpand: equ 72

VDU_BufferBitmapExpandBppMask: equ 3
VDU_BufferBitmapExpandAlignBit: equ 3
VDU_BufferBitmapExpandMappingBufferBit: equ 4

VDU_BufferSpecial: equ $ffff

; Buffer Conditional Commands
VDU_BufferConditionalJump: equ 8

VDU_BufferConditionExists: equ 0
VDU_BufferConditionNotExists: equ 1
VDU_BufferConditionEqual: equ 2
VDU_BufferConditionNotEqual: equ 3
VDU_BufferConditionLessThan: equ 4
VDU_BufferConditionGreaterThan: equ 5
VDU_BufferConditionLessOrEqual: equ 6
VDU_BufferConditionGreaterOrEqual: equ 7
VDU_BufferConditionAnd: equ 8
VDU_BufferConditionOr: equ 9

; Buffer Adjust Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Buffered-Commands-API.md#command-5-adjust-buffer-contents
VDU_BufferAdjust: equ 5

VDU_BufferAdjustNot: equ 0
VDU_BufferAdjustNegate: equ 1
VDU_BufferAdjustSet: equ 2
VDU_BufferAdjustAdd: equ 3
VDU_BufferAdjustAddCarry: equ 4
VDU_BufferAdjustAnd: equ 5
VDU_BufferAdjustOr: equ 6
VDU_BufferAdjustXor: equ 7

VDU_BufferFlagAdvancedOffsets: equ $10
VDU_BufferFlagFetchOperand: equ $20
VDU_BufferFlagMultipleTarget: equ $40
VDU_BufferFlagMultipleOperand: equ $80

; Bitmap Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Bitmaps-API.md
VDU_BitmapCommand: equ 27
VDU_BitmapSelect: equ 0
VDU_BitmapLoad: equ 1
VDU_BitmapSolidColor: equ 2
VDU_BitmapDraw: equ 3
VDU_BitmapSelectBuffer: equ $20
VDU_BitmapCreateBuffer: equ $21
VDU_BitmapFormatRGBA8888: equ 0
VDU_BitmapFormatRGBA2222: equ 1
VDU_BitmapFormatMono: equ 2
VDU_BitmapFormatNative: equ 3

; Standard control characters
CR:	    equ     0dh
LF:	    equ     0ah
NULL:    equ     0       ; end of string (null terminated)

; Output a character
; Params:
;   A = character to output
CharOut:
    rst.lil VDU_SendByte
    ret

; output a string terminated by null
; Params:
;   HL = address of string
StringOut:
    push af
    push bc
    ld bc, 0
    ld a, NULL
    call BytesOut
    pop bc
    pop af
    ret

; Output a specified number of bytes
; Params:
;   HL = address of bytes
;   BC = byte count
BytesOut:
    rst.lis VDU_SendBytes
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
    SendByte VDU_SetMode
    SendByte c
    pop af
    ret

; Reset the VDP context
; Params:
;   C = flags
ResetContext:
    push af
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_ContextManagement
    SendByte VDU_ContextReset
    SendByte c
    pop af
    ret

VdpReset:
    call ResetBitmapChars
    ld de, VDU_BufferSpecial         ; clear all buffers
    call ClearBuffer
    ld c, VDU_ContextResetAll
    call ResetContext
    ld c, VDU_Mode640x480x16         ; switch to mode 0
    call VdpMode
    ret

; Clear the screen
ClearScreen:
    push af
    SendByte VDU_ClearScreen
    pop af
    ret

CursorHome:
    push af
    SendByte VDU_CursorHome
    pop af
    ret

; Move to X, Y coordinates
; Params:
;   C = x coordinate
;   B = Y coordinate
MoveTo:
    push af
    SendByte VDU_CursorMove
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
    SendByte VDU_TextColor       ; pen (foreground)
    SendByte c
    SendByte VDU_TextColor       ; paper (background)
    ld a, b
    add VDU_BackgroundColor
    rst.lis VDU_SendByte
    pop af
    ret

; Hide the text cursor
TextCursorHide:
    push af
    SendByte VDU_Command
    SendByte VDU_CursorDisplay
    SendByte VDU_CursorHide
    pop af
    ret

; Show the text cursor
TextCursorShow:
    push af
    SendByte VDU_Command
    SendByte VDU_CursorDisplay
    SendByte VDU_CursorShow
    pop af
    ret

; Set the specified cursor movement bits
; Params:
;   C = bits to set
SetCursorMovementBit:
    push af
    SendByte VDU_Command
    SendByte VDU_CursorMovement
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
    SendByte VDU_Command
    SendByte VDU_CursorMovement
    SendByte 0
    ld a, c
    cpl
    SendByte a
    pop af
    ret

; Prevent the screen from scrolling when the last character is written
EnableScrollProtection:
    push bc
    ld c, VDU_ScrollProtection
    call SetCursorMovementBit
    pop bc
    ret

; Allow the screen to scroll when the last character is written
DisableScrollProtection:
    push bc
    ld c, VDU_ScrollProtection
    call ClearCursorMovementBit
    pop bc
    ret

; Wait for vsync (and swap buffers if using a buffered video mode)
WaitVSync:
VsyncBufferSwap:
    push af
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_VsyncBufferSwap
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
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_BufferCommand
    SendByte e                  ; buffer id
    SendByte d
    SendByte VDU_BufferClear
    pop af
    ret

; Call a buffer
; Params:
;   DE = buffer id
CallBuffer:
    push af
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_BufferCommand
    SendByte e                  ; buffer id
    SendByte d
    SendByte VDU_BufferCall
    pop af
    ret

; Print buffer contents to screen literally without processing commands
; Params:
;   DE = buffer id
PrintBuffer:
    push af
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_PrintBufferLiteral
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
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_BufferCommand
    SendByte e                  ; buffer id
    SendByte d
    SendByte VDU_BufferWriteBlock
    SendByte c                  ; length
    SendByte b
    rst.lis VDU_SendBytes
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
    SendByte VDU_Command
    SendByte VDU_BitmapCommand
    SendByte VDU_BitmapSelectBuffer
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
    SendByte VDU_Command
    SendByte VDU_BitmapCommand
    SendByte VDU_BitmapCreateBuffer
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
    SendByte VDU_Command
    SendByte VDU_SystemCommand
    SendByte VDU_MapCharToBitmap
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
    ld a, VDU_EscapeChar
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
    SendByte VDU_EscapeChar
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