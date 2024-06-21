
; expand bitmap with arbitrary bpp to 8 bpp
; A = options
; HL = source bitmap ID
; DE = bitmap ID
; BC = bitmap size
; IX = bitmap width
; IY = color address or buffer ID
ExpandBitmap:
; VDU 23, 0, &A0, bufferId; 72, options, sourceBufferId; [width;] <mappingDataBufferId; | mapping-data...>
; https://github.com/AgonConsole8/agon-docs/blob/main/docs/vdp/Buffered-Commands-API.md#command-72-expand-a-bitmap
    push af
    SendByte VduCommand                 ; send command
    SendByte VduBufferCommand
    SendByte e                          ; buffer id
    SendByte d
    SendByte VduBufferBitmapExpand
    pop af                              ; options
    rst.lis VduSendByte
    push af
    SendByte l                          ; source buffer id
    SendByte h
    SendByte ixl                        ; bitmap width
    SendByte ixh
    pop af                              ; calculate number of colors to send
    bit VduBufferBit, a
    jp z, ExpandBitmapSendColor
    SendByte iyl                        ; mapping buffer id
    SendByte iyh
    ret
ExpandBitmapSendMappingBytes:
    push af
    and 3                               ; calculate number of mapping bytes to send
    jp nz, ExpandBitmapBppNot0
    ld a, 8                             ; 0 means 8 bpp
ExpandBitmapBppNot0:
    add a, a
    push hl                             ; send mapping bytes
    push bc
    push iy
    pop hl
    ld bc, 0
    ld c, a
    rst.lis VduSendBytes
    pop bc
    pop hl
    pop af
    ret

; upload vdp draw command
SendDrawCommand:
    ld hl, DrawCommand
    ld de, DrawCommandBuffer
    ld bc, DrawCommandLength
    call ClearAndSendBuffer
    ld hl, DistortLoop
    ld de, DistortLoopBuffer
    ld bc, DistortLoopLength
    call ClearAndSendBuffer
    ret

BitmapDestBufferStart: equ $0
BitmapDestBufferCount: equ 256
BitmapSourceBufferStart: equ $100
BitmapSourceBufferCount: equ 32
ColorSourceBuffer: equ $1ff
DrawCommandBuffer: equ $200
DistortLoopBuffer: equ $201
BaseFrameBuffer: equ $202
TargetFrameBuffer: equ $203
DistortionBuffer: equ $204


; vdp program to distort current frame and draw it based on base frame and distortion value buffers
DrawCommand:
    ; Copy the base frame buffer to the target frame buffer
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw TargetFrameBuffer
    defb VduBufferCopy
    defw BaseFrameBuffer
    defw DistortLoopBuffer
    ; Reset target offset to 0
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw DistortTargetOffset
    defw 2
    defb 0
    ; Reset operand offset to 0
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw DistortOperandOffset
    defw 2
    defb 0
    ; Start the loop to adjust each line
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferCall
    ; Move cursor to home
    defb VduCursorHome
    ; Print the target frame buffer
    defb VduCommand, VduSystemCommand, VduPrintBufferLiteral
    defw TargetFrameBuffer
    ; Wait for vsync and swap buffers
    defb VduCommand, VduSystemCommand, VduVsyncBufferSwap
DrawCommandLength: equ $ - DrawCommand

DistortLoop:
    ; Add an operand from the distortion buffer to all the tiles in a row
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw TargetFrameBuffer      ; buffer id
    defb VduBufferAdjust
    defb VduBufferAdd | VduBufferAdjustFetchOperand | VduBufferAdjustMultiple ; operation
DistortTargetOffset: equ $ - DistortLoop
    defw 0                      ; target buffer offset
    defw ScreenWidth            ; adjust count
    defw DistortionBuffer       ; operand source buffer
DistortOperandOffset: equ $ - DistortLoop
    defw 0                      ; operand source offset
    ; Increment target offset by ScreenWidth
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustAdd
    defw DistortTargetOffset
    defw 1
    defb ScreenWidth
    ; Increment operand offset by 1
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustAdd
    defw DistortOperandOffset
    defw 1
    defb 1
    ; Conditionally jump to the same buffer while operand offset < screen height
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferConditionalJump
    defb VduBufferConditionLessThan
    defw DistortLoopBuffer
    defw DistortOperandOffset
    defb ScreenHeight
DistortLoopLength: equ $ - DistortLoop

    ; set the specified 16 bit value in the specified buffer at the specified offset
    macro SetBuffer16 id, offset, value
    ; low byte
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw id
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw offset
    defw 1
    defb value & $ff
    ; high byte
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw id
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw offset + 1
    defw 1
    defb value >> 8
    endmacro

    ; increment the 16 bit value in the specified buffer at the spe
    macro IncBuffer16 id, offset, increment
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw id
    defb VduBufferAdjust
    defb VduBufferAdjustAddCarry
    defw offset
    defw 1
    defb increment
    endmacro

ExpandCommand:
    ; Reset target offset to 0
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw DistortTargetOffset
    defw 1
    defb BitmapTargetBufferStart & $ff
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw DistortTargetOffset
    defw 1
    defb BitmapTargetBufferStart >> 8
    ; Reset bitmap offset to $100
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw ExpandLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw ExpandBitmapOffset
    defw 1
    defb BitmapSourceBufferStart & $ff
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw ExpandLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw ExpandBitmapOffset
    defw 1
    defb BitmapSourceBufferStart >> 8
    ; Start the loop to adjust each line
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortLoopBuffer
    defb VduBufferCall
ExpandCommandLength: equ $ - ExpandCommand

ExpandLoop:
    ; expand the current bitmap
    defb VduCommand, VduSystemCommand, VduBufferCommand
ExpandTargetOffset: equ $ - ExpandLoop
    defw BitmapDestBuffer
    defb VduBufferBitmapExpand
    defb 1 | VduBufferBitmapExpandMappingBufferBit
ExpandBitmapOffset: equ $ - ExpandLoop
    defw BitmapSourceBufferStart
ExpandMappingOffset:
    defw ColorSourceBuffer
    ; Increment target offset by 1
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw ExpandLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustAddCarry
    defw ExpandTargetOffset
    defw 1
    defb 1
    ; Increment operand offset by 1
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw ExpandLoopBuffer
    defb VduBufferAdjust
    defb VduBufferAdjustAddCarry
    defw ExpandBitmapOffset
    defw 1
    defb 1
    ; Conditionally jump to the same buffer while source buffers remain
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw ExpandLoop
    defb VduBufferConditionalJump
    defb VduBufferConditionLessThan
    defw BitmapSourceBufferStart + BitmapSourceBufferCount
    defw ExpandBitmapOffset
    defb 1
ExpandLoopLength: equ $ - ExpandLoop