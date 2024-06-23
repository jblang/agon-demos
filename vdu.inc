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

; uncomment or comment to enable or disable features

    include "vdu/constants.inc"     ; Constants only; no code
    include "vdu/mos.inc"           ; VDP-related MOS calls

    include "vdu/text.inc"          ; Text commands
    include "vdu/graphics.inc"      ; Graphics commands
    include "vdu/system.inc"        ; System commands

    include "vdu/audio.inc"         ; Audio API
    include "vdu/bitmap.inc"        ; Bitmaps And Sprites API
    include "vdu/buffer.inc"        ; Buffered Commands API
    include "vdu/context.inc"       ; Context Management API
    include "vdu/font.inc"          ; Font Management API

    include "vdu/utility.inc"       ; Utility routines