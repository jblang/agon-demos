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

; Standard control characters
CR:	    equ     0dh
LF:	    equ     0ah
NULL:   equ     0       ; end of string (null terminated)

; VDU commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/VDU-Commands.md
VDU_Null: equ 0
VDU_PrinterChar: equ 1
VDU_PrinterOn: equ 2
VDU_PrinterOff: equ 3
VDU_TextCursor: equ 4
VDU_GraphicsCursor: equ 5
VDU_EnableScreen: equ 6
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
VDU_GraphicsColor: equ 18
VDU_SetPalette: equ 19
VDU_ResetPalette: equ 20
VDU_DisableScreen: equ 21
VDU_SetMode: equ 22
VDU_CustomChar: equ 23
VDU_GraphicsViewport: equ 24
VDU_Plot: equ 25
VDU_ResetViewports: equ 26
VDU_EscapeChar: equ 27
VDU_TextViewport: equ 28
VDU_GraphicsOrigin: equ 29
VDU_CursorHome: equ 30
VDU_CursorMove: equ 31
VDU_Backspace: equ 127

; misc constants
VDU_BackgroundColor: equ 128
VDU_ModeDoubleBuffered: equ 128
VDU_SetPaletteRGB: equ $ff
VDU_Command: equ 23

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