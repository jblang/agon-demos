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

; convenience routine to send common buffer header bytes
VduBufferHeader:
    VduSendByte VDU_Command
    VduSendByte VDU_SystemCommand
    VduSendByte VDU_BufferCommand
    VduSendByte e                   ; buffer id
    VduSendByte d
    ret

; convenience macro to send buffer command header
    macro VduBufferCommand command
    push af
    call VduBufferHeader
    ld a, command
    rst.lis VDU_SendByte
    pop af
    endmacro

; Clear a buffer, then write data to it
VduBufferClearAndWrite:
    call VduBufferClear
    ; fallthrough

; Write a block of data to a buffer
; Params:
;   DE = buffer id
;   BC = buffer length
;   HL = buffer data
VduBufferWrite:
    VduBufferCommand VDU_BufferWriteBlock
    push af
    VduSendByte c                   ; length
    VduSendByte b
    pop af
    push hl
    call VduSendBytes
    pop hl
    ret

; Clear a buffer
; Params:
;   DE = buffer id
VduBufferClear:
    VduBufferCommand VDU_BufferClear
    ret

; Call a buffer
; Params:
;   DE = buffer id
VduBufferCall:
    VduBufferCommand VDU_BufferCall
    ret
