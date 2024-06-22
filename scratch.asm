BaseFrameBufferID: equ $202
TargetFrameBufferID: equ $203
DistortionBufferID: equ $204

; vdp program to distort current frame and draw it based on base frame and distortion value buffers
DrawCommandID: equ $300
DrawCommandStart:
    ; Copy the base frame buffer to the target frame buffer
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw TargetFrameBufferID
    defb VduBufferCopyBlocks
    defw BaseFrameBufferID
    defw VduBufferSpecial
    ; Reset target offset to 0
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortCommandID
    defb VduBufferAdjust
    defb VduBufferAdjustSet
    defw DistortTargetOffset
    defw 2
    defb 0
    ; Reset operand offset to 0
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortCommandID
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
DrawCommandLength: equ $ - DrawCommandStart

DistortCommandID: equ $301
DistortCommandStart:
    ; Add an operand from the distortion buffer to all the tiles in a row
    ; VDU 23, 0, &A0, bufferId; 5, operation, offset; [count;] <operand>, [arguments]
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw TargetFrameBufferID    ; target buffer
    defb VduBufferAdjust
    defb VduBufferAdd | VduBufferAdjustFetchOperand | VduBufferAdjustMultiple ; operation
DistortTargetOffset: equ $ - DistortCommandStart
    defw 0                      ; target buffer offset
    defw ScreenWidth            ; adjust count
    defw DistortionBufferID     ; operand source buffer
DistortOperandOffset: equ $ - DistortCommandStart
    defw 0                      ; operand source offset
DistortCommandLength: equ $ - DistortCommandStart

DistortLoopID: equ $302
DistortLoopStart:
    ; Execute the command to distort the current line
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortCommandID
    defb VduBufferCall
    ; Increment target offset by ScreenWidth
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortCommandID
    defb VduBufferAdjust
    defb VduBufferAdjustAdd
    defw DistortTargetOffset
    defb ScreenWidth
    ; Increment operand offset by 1
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortCommandID
    defb VduBufferAdjust
    defb VduBufferAdjustAdd
    defw DistortOperandOffset
    defb 1
    ; Conditionally jump to the same buffer while operand offset < screen height
    defb VduCommand, VduSystemCommand, VduBufferCommand
    defw DistortCommandID
    defb VduBufferConditionalJump
    defb VduBufferConditionLessThan
    defw DistortCommandID
    defw DistortOperandOffset
    defb ScreenHeight
DistortLoopLength: equ $ - DistortLoopStart

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