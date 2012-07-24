; GAMEGEAR.INC
; General Game Gear equates.

; Memory locations:
.define	rom_bank_1	$0000
.define	rom_bank_2	$4000
.define	ram		$C000
.define vram		$0000
.define vram_tiles_1	$0000	; Tiles 0..255
.define vram_tiles_2	$2000	; Tiles 256..447
.define vram_names	$3800	; Name table
.define	vram_sprites	$3F00	; Sprite info table

; VDP ports:
.define	vdp_data	$BE
.define	vdp_inst	$BF
.define	vdp_vcount	$7E

; VDP registers:
.define	vdp_reg_mode_1		$00	; Mode 1 register 
.define	vdp_reg_mode_2		$01	; Mode 2 register
.define	vdp_reg_name_loc	$02	; Address of name table
.define	vdp_reg_overscan	$07	; Overscan colour (lower four bits = colour)
.define	vdp_reg_scroll_x	$08	; X-Scrolling offset
.define	vdp_reg_scroll_y	$09	; Y-Scrolling offset
.define	vdp_reg_line_counter	$0A	; Line counter interrupt register

.define	psg_port	$7F

; VDP mode 1 bitmasks:

.define	vdp_mode_bw	%00000001	; Monochrome display, no synch
.define	vdp_mode_height	%00000010	; Allow screen height changes
.define	vdp_mode_mode_4	%00000100	; Use mode 4
.define	vdp_mode_move_8	%00001000	; Shift sprites left 8 pixels
.define vdp_mode_line	%00010000	; Line interrupt mode
.define vdp_mode_msk_0	%00100000	; Mask column 0 with overscan colour

; VDP mode 2 bitmasks:
.define	vdp_mode_zoom	%00000001	; Zoom sprites to 16x16
.define	vdp_mode_double	%00000010	; Double (pair) sprites to be 16x8
.define	vdp_mode_30	%00001000	; 30 row / 240 line mode
.define	vdp_mode_240	%00001000	; 30 row / 240 line mode
.define	vdp_mode_28	%00010000	; 28 row / 224 line mode
.define	vdp_mode_224	%00010000	; 28 row / 224 line mode
.define	vdp_mode_vblank	%00100000	; Vblank interrupts
.define	vdp_mode_enable	%01000000	; Enable the display


; Joystick:
.define joystick	$DC
.define	start		$00

.define	joy_up		%00000001
.define	joy_down	%00000010
.define	joy_left	%00000100
.define	joy_right	%00001000
.define	joy_1		%00010000
.define	joy_2		%00100000

.define	start_button	%10000000