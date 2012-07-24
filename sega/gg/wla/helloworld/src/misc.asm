; MISC.ASM
; A variety of helpful, non-Game Gear (but can be Fire Track)
; specific routines.

;==============================================================
; SWITCH SCREEN ON/OFF
;==============================================================

screen_on:
	ld a,vdp_mode_enable|vdp_mode_double
.ifdef SUPER_SIZE_ME
	or vdp_mode_zoom
.endif
	jr +
screen_off:
	xor a
+:	ld b,vdp_reg_mode_2
	call vid_set_vdp_reg
	ret

;==============================================================
; COMPARE HL & DE: call_hl_de WORKS LIKE "cp de", WHERE A = HL
;==============================================================

cp_hl_de:
	push hl
	scf
	ccf
	sbc hl,de 
	pop hl
	ret

;==============================================================
; RETURNS LOCATION ON NAME TABLE BASED ON SCREEN (x,Y) COORD
;==============================================================

get_name_table_loc:


	ld a,(cur_scroll_index)
	cp 199
	jr z,+
	jr c,+
	sub 224
+:	add a,24
	ld l,a
	ld h,0
	ld d,0
	ld e,c
	add hl,de

	srl h
	rr l
	srl h
	rr l
	srl h
	rr l
	
	ld a,l
	dec a
	cp -1
	jr nz,+
	xor a
+:

	cp 28
	jr c,+
	sub 28
+:	ld l,a
	ld h,0
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	ld a,b
	srl a
	srl a
	srl a
	ld e,a
	ld d,0
	add hl,de
	add hl,hl
	ret

;==============================================================
; LIKE THE TI-OS FUNCTION: SETS B BYTES AFTER HL TO VALUE A
;==============================================================

mem_set:
	ld (hl),a
	inc hl
	djnz mem_set
	ret

;==============================================================
; LD HL,(HL)
;==============================================================

ld_hl_hl:
	push ix
	push hl
	pop ix
	ld h,(ix+1)
	ld l,(ix+0)
	pop ix
	ret

;==============================================================
; LOAD THE BBC MICRO FONT
;==============================================================

load_font:
	ld hl,font
	ld de,fontend-font
	ld bc,0
	ld a,1
	call vid_copy_tileset
	ret

;==============================================================
; DELAY FOR A 'B' NUMBER OF FRAMES
;==============================================================

delay_for_x_frames:
	push bc
	call vid_wait_vblank
	pop bc
	djnz delay_for_x_frames
	ret

;==============================================================
; WAIT FOR THE START BUTTON TO BE PRESSED THEN RELEASED
;==============================================================

wait_start:
	in a,(start)
	and start_button
	jr nz,wait_start
-:	in a,(start)
	and start_button
	jr z,-
	ret

;==============================================================
; RETURN RANDOM NUMBER BETWEEN 0 AND B
;==============================================================

random:
	push hl
	push de
	ld hl,(rand_data)
	ld a,r
	ld d,a
	ld e,(hl)
	add hl,de
	add a,l
	xor h
	ld (rand_data),hl
	sbc hl,hl
	ld e,a
	ld d,h
-:
	add hl,de
	djnz -
	ld a,h
	pop de
	pop hl
	ret

;==============================================================
; RLE: LIKE LDIR BUT WORKS ON RLE COMPRESSED DATA
;==============================================================

rle:
	ld a,(hl)
	cp $FF
	jr z,rle_run
	ldi
--:
	ret po
	jr rle
rle_run:
	inc hl
	inc hl

	ld a,(hl)
-:
	dec hl
	dec a
	ldi
	jr nz,-
	inc hl
	jr --


;==============================================================
; DISPLAY VALUE OF ACCUMULATOR AS TWO-DIGIT DECIMAL (00..99)
;==============================================================

disp_a:
	ld c,a
	ld l,0	; 10s


-:	cp 10
	jr c,+
	inc l
	sub 10
	jr -
+:

	; l = number of 10s.

	ld a,l
	add a,a
	ld b,a
	add a,a
	add a,a
	add a,b
	; a = 10*l

	ld b,a

	ld a,c
	sub b

	ld h,a

	; h = units.


	ld a,l
	add a,'0'-$20
	out (vdp_data),a
	xor a
	out (vdp_data),a

	ld a,h
	add a,'0'-$20
	out (vdp_data),a
	xor a
	out (vdp_data),a
	ret

;==============================================================
; PUT ZERO-TERMINATED STRING FROM HL AT LOCATION IN DE
;==============================================================

puts_tab:
	push hl
	ex de,hl
	call vid_goto_address
	pop hl

;==============================================================
; PUT ZERO-TERMINATED STRING FROM HL
;==============================================================

puts:	ld a,(hl)
	or a
	ret z
	sub $20
	out (vdp_data),a
	xor a
	out (vdp_data),a
	inc hl
	jr puts

;==============================================================
; RESET SCORE Y
;==============================================================

reset_score_y:
	; Reset score sprite locations
	call vid_wait_vblank
	ld hl,vram_sprites	
	call vid_goto_address
	ld a,16
	ld b,8
-:	out (vdp_data),a
	djnz -
	ret