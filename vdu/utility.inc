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

; VDP-related utility functions
VdpReset:
    call ResetBitmapChars
    ld de, VDU_BufferSpecial         ; clear all buffers
    call VduBufferClear
    ld c, VDU_ContextResetAll
    call ResetContext
    ld c, VDU_Mode640x480x16         ; switch to mode 0
    call VdpMode
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
    VduSendByte a
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