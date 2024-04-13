; AgonLight convenience macros
; Originally from MyMacros.inc by Richard Turnnidge
; https://github.com/richardturnnidge/lessons

	macro MosCall arg1
	ld a, arg1
	rst.lil $08
	endmacro
	
	macro VdpMode mode
	ld a, 22					; set mode...
	rst.lil $10
	ld a, mode					; to mode
	rst.lil $10
	endmacro

	macro ClearScreen
	ld a, 12
	rst.lil $10
	endmacro

	macro NewLine
    ld a, 13					; carriage return
    rst.lil $10
    ld a, 10					; line feed
    rst.lil $10
	endmacro
	
	macro MoveTo x,y
	ld a, 31					; move to...
	rst.lil $10
	ld a, x						; X position
	rst.lil $10
	ld a, y						; Y position
	rst.lil $10
	endmacro

	macro PenColor value
	ld a, 17					; set text colour
	rst.lil $10
	ld a, value					; colour to use
	rst.lil $10
	endmacro

	macro PaperColor value
	ld a, 17					; set text colour
	rst.lil $10
	ld a, value					; colour to use
	add a, 128
	rst.lil $10
	endmacro

	macro SendBytes start, length
    ld hl, start
    ld bc, length
    rst.lil $18
    endmacro
    
