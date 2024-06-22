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

; System Commands
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/System-Commands.md

; Set the VDP mode
; Params:
;   C = mode number
VdpMode:
    push af
    SendByte VDU_SetMode
    SendByte c
    pop af
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
