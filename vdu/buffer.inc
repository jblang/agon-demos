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

; Clear a buffer
; Params:
;   DE = buffer id
ClearBuffer:
    push af
    VduSendByte VDU_Command
    VduSendByte VDU_SystemCommand
    VduSendByte VDU_BufferCommand
    VduSendByte e                  ; buffer id
    VduSendByte d
    VduSendByte VDU_BufferClear
    pop af
    ret

; Call a buffer
; Params:
;   DE = buffer id
CallBuffer:
    push af
    VduSendByte VDU_Command
    VduSendByte VDU_SystemCommand
    VduSendByte VDU_BufferCommand
    VduSendByte e                  ; buffer id
    VduSendByte d
    VduSendByte VDU_BufferCall
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
    VduSendByte VDU_Command
    VduSendByte VDU_SystemCommand
    VduSendByte VDU_BufferCommand
    VduSendByte e                  ; buffer id
    VduSendByte d
    VduSendByte VDU_BufferWriteBlock
    VduSendByte c                  ; length
    VduSendByte b
    rst.lis VDU_SendBytes
    pop de
    pop bc
    pop hl
    pop af
    ret
