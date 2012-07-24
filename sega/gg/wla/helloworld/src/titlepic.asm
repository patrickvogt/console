; TITLEPIC.ASM
; Draws the title pic

display_title_pic:
	call screen_off

	call vid_clear_vram


	xor a
	ld b,vdp_reg_scroll_x
	call vid_set_vdp_reg

	xor a
	ld b,vdp_reg_scroll_y
	call vid_set_vdp_reg


	ld hl,vram_names+12+64*3
	call vid_goto_address

	ld b,18

	ld hl,title_pic_pic

--:	push bc

	ld b,20
-:	ld a,(hl)
	out (vdp_data),a
	xor a
	out (vdp_data),a
	inc hl
	djnz -

	ld b,(32-20)*2
	xor a
-:	out (vdp_data),a
	djnz -


	pop bc
	djnz --

	ld hl,title_pic_tiles
	ld de,title_pic_tiles_end-title_pic_tiles
	ld a,4
	ld bc,0
	call vid_copy_tileset

	ld hl,title_pic_palette
	xor a
	ld b,32
	call vid_load_palette
	call screen_on

	call wait_start