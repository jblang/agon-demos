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

; Convenience wrappers for MOS calls to interact with VDP

; Send a single byte to the CPU
; Caller must push/pop af if necessary
    macro VduSendByte value
        ld a, value
        rst.lis VDU_SendByte
    endmacro

; Send bytes to VDU
;   HL = address of bytes to send
;   BC = number of bytes to send
; Returns:
;   HL = next address after bytes sent
VduSendBytes:
    push af
    push bc
    push de
    rst.lis VDU_SendBytes
    pop de
    pop bc
    pop af
    ret

; Send a character to VDU
;   A = char to send
VduSendChar:
    rst.lis VDU_SendBytes
    ret

; Send null-terminated string to VDU
;   HL = address of string to send
; Returns:
;   HL = next address after string terminator
VduSendString:
    push af
    push bc
    push de
    ld bc, 0
    ld a, NULL
    rst.lis VDU_SendBytes
    pop de
    pop bc
    pop af
    inc hl      ; move past terminator
    ret