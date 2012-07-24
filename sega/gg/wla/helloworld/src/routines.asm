; ROUTINES.ASM
; A nifty bunch of useful Game Gear graphics related routines


; vid_reset
;	Initialises the VDP for use.

; vid_goto_address
;	Sets VDP write to address in HL
;	In: HL

; vid_load_palette
;	Loads the palette at address HL, size of B into the CRAM starting from index A
;	In: HL is the address of the palette, B is the size (bytes), A is the offset in CRAM

; vid_get_vcount
;	Returns the vertical line count in the accumulator.
;	Out: A and B are the vertical count.

; vid_copy_tileset
;	Copies a tileset from the cart to VRAM
;	In: HL is the address of the tileset.
;	    DE is the size of the tileset (bytes)
;	    A  is the number of bits-per-pixel in the tileset [1,2,3,4]
;	    BC is the tile offset in VRAM

; vid_clear_vram
;	Clear all VRAM

; vid_write_blanks
;	Writes "B" number of zeroes to the VDP
;	In: B is the number of zeroes to write

; vid_set_vdp_reg
;	Sets VDP register B to value A
;	In: A is the value to write to register number B

; vid_destroy_sprites
;	Erases ALL sprites

vid_reset:
	ld hl,vdp_reset_data
	ld b,vdp_reset_data_end-vdp_reset_data
	ld c,vdp_inst
	otir
	ret

vdp_reset_data:		.db $04,$80,$84,$81,$FF,$82,$FF,$85,$FF,$86,$FF,$87,$00,$88,$00,$89,$FF,$8A
vdp_reset_data_end:


vid_goto_address:
	ld a,l
	out (vdp_inst),a
	ld a,h
	or $40
	out (vdp_inst),a
	ret

vid_goto_palette:
	add a,a
	out (vdp_inst),a
	ld a,$C0
	out (vdp_inst),a
	ret

vid_load_palette:
	push bc
	push hl
	ld e,a
	ld d,0
	add hl,de
	ld de,pal_from
	ld c,b
	ld b,0
	ldir
	pop hl
	pop bc	
	out (vdp_inst),a
	ld a,$C0
	out (vdp_inst),a
	ld c,vdp_data
	otir
	ret

vid_get_vcount:
	in a,($7E)
	ld b,a
	in a,($7E)
	cp b
	jr nz,vid_get_vcount
	ret

vid_copy_tileset:
	push hl
	ld h,b
	ld l,c
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	ld c,a
	call vid_goto_address
	pop hl

vid_copy_tileset_loop:
	ld b,c
vid_copy_tileset_write_data_loop:
	ld a,(hl)
	inc hl
	dec de
	out (vdp_data),a
	djnz vid_copy_tileset_write_data_loop
	
	ld a,4
	sub c
	or a
	jr z,no_need_skip_planes
	ld b,a
	xor a
	
skip_planes_loop:
	out (vdp_data),a
	djnz skip_planes_loop

no_need_skip_planes:
	ld a,d
	or e
	jr nz,vid_copy_tileset_loop
	ret
	

vid_clear_vram:
	ld hl,$0000
	call vid_goto_address
	ld bc,$4000
vid_clear_ram_loop:
	xor a
	out (vdp_data),a
	dec bc
	ld a,b
	or c
	jr nz,vid_clear_ram_loop

	xor a
	call vid_goto_palette
	ld b,32*2
	xor a
-:	out (vdp_data),a
	djnz -

	ret


vid_set_vdp_reg:
	out (vdp_inst),a
	ld a,%10000000
	or b
	out (vdp_inst),a
	ret


vid_write_blanks:
	xor a
vid_write_blanks_loop:
	out (vdp_data),a
	djnz vid_write_blanks_loop
	ret

vid_wait_vblank:
	call vid_get_vcount
	cp $C0
	jr nz,vid_wait_vblank
	ret

vid_destroy_sprites:
	ld hl,vram_sprites
	call vid_goto_address
	ld b,64
	ld a,$D0
-:	out (vdp_data),a
	djnz -


	ret