; executable preamble
    
    .assume adl=1                           ; ez80 ADL memory mode
    .org $40000                             ; load code here

    jp Entry                                ; jump to start of code

    .align 64                               ; MOS header
    .db "MOS",0,1     

    include "vdu.inc"

Entry:
    push af                                 ; store all the registers
    push bc
    push de
    push ix
    push iy

    call Main

    pop iy                                  ; pop all registers back from the stack
    pop ix
    pop de
    pop bc
    pop af
    ld hl,0                                 ; load the MOS API return code (0) for no errors.
    ret                                     ; return to MOS

Main:
    call InitBuffers
    ld de, DrawLoopBuffer
    call VduBufferCall
    call NewLine
    ret

; upload command buffers
InitBuffers:
    ld hl, DrawLoop
    ld de, DrawLoopBuffer
    ld bc, DrawLoopLength
    call VduBufferClearAndWrite
    ld hl, Draw
    ld de, DrawBuffer
    ld bc, DrawLength
    call VduBufferClearAndWrite
    ret


; command buffer to output a single character
DrawBuffer: equ 0
Draw:
    defb VDU_EscapeChar
DrawCharOffset: equ $ - Draw
    defb ' '
DrawLength: equ $ - Draw


; command buffer to output characters 0-255
DrawLoopBuffer: equ 1
DrawLoop:
    ; Escape the character and output it
DrawLoopCharOffset: equ $ - DrawLoop
    ; call the buffer that will draw a character
    defb VDU_Command, VDU_SystemCommand, VDU_BufferCommand ; 23, 0, &A0
    defw DrawBuffer         ; target
    defb VDU_BufferCall      ; command
    ; increment the character that will be drawn next
    defb VDU_Command, VDU_SystemCommand, VDU_BufferCommand ; 23, 0, &A0
    defw DrawBuffer         ; target
    defb VDU_BufferAdjust    ; command
    defb VDU_BufferAdjustAdd ; operation
    defw DrawCharOffset     ; offset
    defb 1                  ; operand
    ; repeat until the character loops back around to 0
    defb VDU_Command, VDU_SystemCommand, VDU_BufferCommand ; 23, 0, &A0
    defw DrawLoopBuffer
    defb VDU_BufferConditionalJump   ; command
    defb VDU_BufferConditionNotEqual ; operation
    defw DrawBuffer                 ; source buffer
    defw DrawCharOffset             ; offset
    defb 0                          ; value
DrawLoopLength: equ $ - DrawLoop