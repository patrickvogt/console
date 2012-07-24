; TITLE.ASM

	; SWITCH OFF SCREEN

	call screen_off
	call vid_clear_vram


	; Copy over the tiles:
	ld hl,grfx_title
	ld de,grfx_title_end-grfx_title
	ld a,4
	ld bc,0
	call vid_copy_tileset

	ld a,$FF
	ld b,8*4

-:
	out (vdp_data),a
	djnz -

	; RESET SCROLL
	xor a
	ld b,vdp_reg_scroll_y
	call vid_set_vdp_reg


	ld hl,vram_names+12+(6*32)
	call vid_goto_address

	ld hl,map_title


	ld a,18
--:
	push af

	ld b,32
-:
	ld a,(hl)
	inc hl
	out (vdp_data),a
	xor a
	out (vdp_data),a
	djnz -

	ld de,-12
	add hl,de

	call vid_wait_vblank

	pop af
	dec a
	jr nz,--

	xor a
	out (vdp_inst),a
	ld a,$C0
	out (vdp_inst),a

	ld b,12
	
-:	ld a,%00001111
	out (vdp_data),a
	xor a
	out (vdp_data),a
	djnz -




	; START THE TONES:
	
	; #0: Solid single freq
	ld a,%10000100
	out (psg_port),a
	ld a,%00110110
	out (psg_port),a

	; #1: Slightly (ever so) different freq to #0 for beats
	ld a,%10100101
	out (psg_port),a
	ld a,%00110110
	out (psg_port),a

	; #2: Slightly (ever so) different freq to #1 for beats
	ld a,%11000110
	out (psg_port),a
	ld a,%00110110
	out (psg_port),a

	ld a,15
	ld (game_clock+1),a
	call update_title_vol



	ld a,vdp_mode_enable
	ld b,vdp_reg_mode_2
	call vid_set_vdp_reg


	; Fade in the text:



-:	ld b,6
	call title_delay_for_x_frames
	
	ld a,(game_clock+1)
	dec a
	cp -1
	jr z,hit_loudest
	ld (game_clock+1),a
	call update_title_vol
hit_loudest:


	ld a,(title_fade_text)
	inc a
	ld (title_fade_text),a
	cp 16
	jr z,+

	ld b,a
	xor a
	call vid_goto_palette

	ld a,%00001111
	out (vdp_data),a
	ld a,b
	out (vdp_data),a

	ld a,11
	call vid_goto_palette
	ld a,b
	add a,a
	add a,a
	add a,a
	add a,a
	or %00001111
	out (vdp_data),a

	ld a,b
	out (vdp_data),a

	jr -


+:	
	; Delay before adding shadows:
	ld b,80
	call title_delay_for_x_frames


	; Finished filling in the text
	; Time to fade in the shadow of the letters...
	xor a
	ld (title_fade_text),a

-:	ld b,8
	call title_delay_for_x_frames
	ld hl,pal_title
	ld a,(title_fade_text)
	inc a
	ld (title_fade_text),a
	cp 9
	jr z,+
	add a,a
	ld e,a
	ld d,0
	add hl,de


	ld a,1
	call vid_goto_palette
	ld b,10
	
--:	ld a,(hl)
	out (vdp_data),a
	inc hl
	ld a,(hl)
	out (vdp_data),a
	inc hl
	djnz --
	

	jr -


+:
	
wait_start_title:


-:	ld b,26
	call title_delay_for_x_frames

	ld a,(game_clock+1)
	inc a
	cp 16
	jr z,hit_quietest
	ld (game_clock+1),a
	call update_title_vol
	jr -

hit_quietest:


	; Draw those black bars:
	xor a
	ld (game_clock),a


-:	ld b,8
	call delay_for_x_frames
	ld a,(game_clock)
	call draw_vertical_bar
	ld a,(game_clock)
	ld b,a
	ld a,19
	sub b
	call draw_vertical_bar
	
	ld a,(game_clock)
	inc a
	ld (game_clock),a
	cp 10
	jr nz, -


	xor a
	ld b,vdp_reg_mode_2
	call vid_set_vdp_reg

	jr exit_title

noise_chan:
	.db %11100000
	.db %11100001


title_delay_for_x_frames:
	push hl
	push de
	push af
-:
	push bc
	call vid_wait_vblank
	call update_title_sound
	pop bc
	djnz -
	pop af
	pop de
	pop hl
	ret

update_title_sound:
	ld a,(game_clock)
	inc a
	ld (game_clock),a
	srl a
	srl a
	srl a
	and %00000001
	ld l,a
	ld h,0
	ld de,noise_chan
	add hl,de
	ld a,(hl)
	out (psg_port),a
	ret


update_title_vol:
	ld b,a
	or %10010000
	out (psg_port),a
	or %00100000
	out (psg_port),a
	ld a,b
	or %11010000
	out (psg_port),a
	ld a,b
	or %11110000
	out (psg_port),a
	ret

draw_vertical_bar:
	; a= x coordinate
	add a,a
	ld l,a
	ld h,0
	ld de,vram_names+12
	add hl,de
	

	ld b,28
	ld de,64
-:	push hl
	call vid_goto_address
	ld a,22
	out (vdp_data),a
	xor a
	out (vdp_data),a
	pop hl
	add hl,de
	djnz -
	ret

exit_title: