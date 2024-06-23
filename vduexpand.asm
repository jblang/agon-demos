; executable preamble
    
    .assume adl=1                           ; ez80 ADL memory mode
    .org $40000                             ; load code here

    jp Entry                                ; jump to start of code

    .align 64                               ; MOS header
    .db "MOS",0,1     

    include "vdu.inc"
    include "data.inc"
    include "gradient.inc"

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
    call SendGradientPatterns
    ld hl, C64PaletteRGBA2222
    ld ix, ColorPalettes
    call SendGradientColors
    ld hl, GradientPatternStartID+16
    ld bc, GradientColorStartID
    ld a, 1 | VDU_BufferBitmapExpandMappingBufferBit
    call VduBufferBitmapExpand
    ret

ColorPalette:
        defb    0
