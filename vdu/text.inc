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

; Text Functions
; Output a character
; Params:
;   A = character to output
CharOut:
    rst.lil VDU_SendByte
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
    VduSendByte VDU_EscapeChar
    pop af
EscapeCharNo:                   ; if not, just send char
    VduSendByte a
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
    VduSendByte ' '
    pop af
    ret

; Output a new line
NewLine:
    push af
    VduSendByte CR
    VduSendByte LF
    pop af
    ret

; Clear the screen
ClearScreen:
    push af
    VduSendByte VDU_ClearScreen
    pop af
    ret

CursorHome:
    push af
    VduSendByte VDU_CursorHome
    pop af
    ret

; Move to X, Y coordinates
; Params:
;   C = x coordinate
;   B = Y coordinate
MoveTo:
    push af
    VduSendByte VDU_CursorMove
    VduSendByte c
    VduSendByte b
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
    VduSendByte VDU_TextColor       ; pen (foreground)
    VduSendByte c
    VduSendByte VDU_TextColor       ; paper (background)
    ld a, b
    add VDU_BackgroundColor
    rst.lis VDU_SendByte
    pop af
    ret

; Hide the text cursor
TextCursorHide:
    push af
    VduSendByte VDU_Command
    VduSendByte VDU_CursorDisplay
    VduSendByte VDU_CursorHide
    pop af
    ret

; Show the text cursor
TextCursorShow:
    push af
    VduSendByte VDU_Command
    VduSendByte VDU_CursorDisplay
    VduSendByte VDU_CursorShow
    pop af
    ret

; Set the specified cursor movement bits
; Params:
;   C = bits to set
SetCursorMovementBit:
    push af
    VduSendByte VDU_Command
    VduSendByte VDU_CursorMovement
    VduSendByte c
    ld a, c
    cpl
    VduSendByte a
    pop af
    ret

; Clear the specified cursor movement bits
; Params:
;   C = bits to clear
ClearCursorMovementBit:
    push af
    VduSendByte VDU_Command
    VduSendByte VDU_CursorMovement
    VduSendByte 0
    ld a, c
    cpl
    VduSendByte a
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

