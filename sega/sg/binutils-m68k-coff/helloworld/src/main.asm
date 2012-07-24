.text
.globl main

##################################################
#        Register and bitmask definitions        #
##################################################

.equ GFXDATA, 		0xc00000
.equ GFXCNTL, 		0xc00004

.equ VDP0_E_HBI, 	0x10
.equ VDP0_E_DISPLAY, 	0x02 
.equ VDP0_PLTT_FULL, 	0x04 

.equ VDP1_SMS_MODE,	0x80
.equ VDP1_E_DISPLAY,	0x40
.equ VDP1_E_VBI,	0x20
.equ VDP1_E_DMA,	0x10
.equ VDP1_NTSC,		0x00
.equ VDP1_PAL,		0x08
.equ VDP1_RESERVED,	0x04

.equ VDP12_SPR_SHADOWS,	0x08
.equ VDP12_SCREEN_V224,	0x00
.equ VDP12_SCREEN_V448,	0x04
.equ VDP12_PROGRESSIVE,	0x00
.equ VDP12_INTERLACED,	0x02
.equ VDP12_SCREEN_H256,	0x00
.equ VDP12_SCREEN_H320,	0x81

.equ VDP16_MAP_V32,	0x00
.equ VDP16_MAP_V64,	0x10
.equ VDP16_MAP_V128,	0x30
.equ VDP16_MAP_H32,	0x00
.equ VDP16_MAP_H64,	0x01
.equ VDP16_MAP_H128,	0x03

##################################################
#                   MACROS                       #
##################################################

/* Write val to VDP register reg */
.macro write_vdp_reg reg val
	move.w #((\reg << 8) + 0x8000 + \val),(a3)
.endm

/* For immediate addresses */
.macro VRAM_ADDR reg adr
	move.l #(((0x4000 + (\adr & 0x3fff)) << 16) + (\adr >> 14)),\reg
.endm

/* For indirect (variable) addresses.
   Destroys d6-d7. */
.macro VRAM_ADDR_var reg adr
	move.l \adr,d6
	move.l \adr,d7
	and.w #0x3fff,d6
	lsr.w #7,d7
	lsr.w #7,d7
	add.w #0x4000,d6
	lsl.l #7,d6
	lsl.l #7,d6
	lsl.l #2,d6
	or.l d7,d6
	move.l d6,\reg
.endm

.macro CRAM_ADDR reg adr
	move.l	#(((0xc000 + (\adr & 0x3fff)) << 16) + (\adr >> 14)),\reg
.endm

/* For indirect (variable) addresses */
.macro CRAM_ADDR_var reg adr
	move.l \adr,d6
	move.l \adr,d7
	and.w #0x3fff,d6
	lsr.w #7,d7
	lsr.w #7,d7
	add.w #0xc000,d6
	lsl.l #7,d6
	lsl.l #7,d6
	lsl.l #2,d6
	or.l d7,d6
	move.l d6,\reg
.endm

##################################################
#               MAIN PROGRAM                     #
##################################################
 
main:
	move.l		#1,-(a7)
	move.l		#0,-(a7)
	addq.l		#8,a7		/* Pop arguments */

	move.l		#0,framectr
	move.l		#0,scrollchar
	
	/* Initialize VDP */
	jsr 		init_gfx

	/* Wait a few frames */	
	jsr		wait_vsync
	jsr		wait_vsync
	jsr		wait_vsync

	/* Load font tiles (64 16x16 characters => 256 tiles) */
	move.l		#(0x0000 + 413*32),a3
	move.l 		#tiles,a4
	move.l		#256,d4
	jsr		load_tiles_rle

	/* Set colors 0 and 1 of the 4th palette (used by the
	   text scroller). The relevant addresses will already
	   have been set by the last call to load_colors. */
	move.w		#0x0000,(a3)	/* Black */
	move.w		#0x0eee,(a3)	/* White */

	jsr		wait_vsync

	/* Clear map B */
	move.l		#0xc000,a0
	move.l		#(0xe000+413),d0
	jsr		clear_map
	
	jsr		wait_vsync
  
	/* Copy sprite data from ROM to RAM/VRAM */
	move.l 		#GFXCNTL,a3
	VRAM_ADDR 	d0,0xfc00
	move.l 		d0,(a3)
	move.l 		#GFXDATA,a3
	move.l		#sprite_data,a4
	move.l		#sprite_data_ram,a5
	move.l		#20,d1
	
	move.l		#0x0eee,textcolor
	move.l		#0x000f,dtextc
	move.l		#0,counter3

##################################################
#                 MAIN LOOP                      #
##################################################

forever:
	jsr		wait_vsync

	/* During the first 220 frames we will add new stuff
	   to the screen. After that we'll just be updating
	   the text scroller and the sprite positions. */
	cmp.l		#220,framectr
	bgt		dont_inc_anymore
	addq.l		#1,framectr

	jmp		dont_inc_anymore2

dont_inc_anymore:

	/* Here we move the sprites. There are actually only
	   two "characters", but since they are rather large
	   they need a total of five sprites. The code for
	   changing the sprites' Y-position is quite ugly.. */
	   
	move.l 		#GFXDATA,a6
	
	addq.b		#3,spritean
	move.b		spritean,d6
	and.l		#0xff,d6

	move.l		d6,a4
	move.l		#sprite_data,a5
  
	jsr		draw_string
  
/* Fade scroller color from white->green->white->green etc */
	move.l		#0x0eee,d0
	
	move.l 		#GFXCNTL,a3
	CRAM_ADDR 	d0,0x0062
	move.l 		d0,(a3)
	move.l 		#GFXDATA,a3
	move.l		textcolor,d0
	move.w		d0,(a3)

dont_inc_anymore2:	
	bra 		forever
	
#################################################
#         Initialize VDP registers              #
#################################################

init_gfx:
	move.l 		#GFXCNTL,a3
	write_vdp_reg 	0,(VDP0_E_HBI + VDP0_E_DISPLAY + VDP0_PLTT_FULL)
	write_vdp_reg 	1,(VDP1_E_VBI + VDP1_E_DISPLAY + VDP1_E_DMA + VDP1_PAL + VDP1_RESERVED)
	write_vdp_reg 	2,(0xe000 >> 10)	/* Screen map a adress */
	write_vdp_reg 	3,(0xe000 >> 10)	/* Window address */
	write_vdp_reg 	4,(0xc000 >> 13)	/* Screen map b address */
	write_vdp_reg 	5,(0xfc00 >>  9)	/* Sprite address */
	write_vdp_reg 	6,0	
	write_vdp_reg	7,0			/* Border color */
	write_vdp_reg	8,1			/* Unused (?) */
	write_vdp_reg	9,1			/* Unused (?) */
	write_vdp_reg	10,1			/* Lines per hblank interrupt */
	write_vdp_reg	11,4			/* 2-cell vertical scrolling */
	write_vdp_reg	12,(VDP12_SCREEN_V224 + VDP12_SCREEN_H256 + VDP12_PROGRESSIVE)
	write_vdp_reg	13,(0x6000 >> 10)	/* Horizontal scroll address */
	write_vdp_reg	15,2
	write_vdp_reg	16,(VDP16_MAP_V32 + VDP16_MAP_H64)
	write_vdp_reg	17,0
	write_vdp_reg	18,0xff
	rts

#################################################
#    Draw a string of 11 characters on map B    #
#     The characters are 16x16 pixels each      #
#################################################

draw_string:
	move.l 		#GFXCNTL,a4
	VRAM_ADDR 	d0,0xc580
	move.l 		d0,(a4)
	move.l 		#GFXDATA,a3
	move.l		scrollchar,d5
	move.l		#Message,a2
	add.l		d5,a2
	move.l		#0,d6
_draw_char_upper:
	move.b		(a2)+,d0
	cmp.b		#0,d0
	bne		_no_wrap1
	move.l		#Message,a2
	bra 		_draw_char_upper
_no_wrap1:
	addq.w		#1,d5
	and.l 		#0xff,d0
	sub.b		#32,d0
	lsl.w		#2,d0
	add.w		#413,d0		/* 413 = first char in font */
	or.w		#0xe000,d0

	move.w		d0,(a3)
	addq.w		#1,d0
	move.w		d0,(a3)
	addq.w		#1,d6
	cmp.b		#11,d6
	bne 		_draw_char_upper

	VRAM_ADDR 	d0,0xc600
	move.l 		d0,(a4)
	move.l		scrollchar,d5
	move.l		#Message,a2
	add.l		d5,a2
	move.l		#0,d6
_draw_char_lower:
	move.b		(a2)+,d0
	cmp.b		#0,d0
	bne		_no_wrap2
	move.l		#Message,a2
	bra 		_draw_char_lower
_no_wrap2:
	addq.w		#1,d5
	and.l 		#0xff,d0
	sub.b		#32,d0
	lsl.w		#2,d0
	add.w		#415,d0		/* 413 = first char in font */
	or.w		#0xe000,d0
	move.w		d0,(a3)
	addq.w		#1,d0
	move.w		d0,(a3)
	addq.w		#1,d6
	cmp.b		#11,d6
	bne 		_draw_char_lower
	
	rts

#################################################
#    Copy part of a 32x28 nametable to VRAM     #
#                                               #
# Parameters:                                   #
#  a2: nametable                                #
#  a3: nametable x                              #
#  a4: nametable y                              #
#  a5: tile number offset                       #
#  d0: map base                                 #
#  d1: map x                                    #
#  d2: map y                                    #
#  d3: columns                                  #
#  d4: rows                                     #
#################################################

copy_nametable_part:
	move.l 		d3,counter1
	move.l 		d4,counter2
	
	move.l 		#GFXCNTL,a6
	lsl.l		#7,d2
	lsl.l		#1,d1
	add.l		d2,d0
	add.l		d1,d0		/* d0 = map base + map y * 128 + map x * 2 */
	
	move.l		a3,d1
	move.l		a4,d2
	lsl.l		#6,d2
	lsl.l		#1,d1
	add.l		d2,a2
	add.l		d1,a2

	move.l 		#GFXDATA,a3
	
	move.l		#32,d2
	sub.l		d3,d2		/* 32 - columns */
	lsl.w		#1,d2
_set_rows:
	VRAM_ADDR_var	d5,d0
	move.l 		d5,(a6)		/* Set VRAM address */
	move.l 		counter1,a4	/* Copy number of columns */
_set_columns:
	move.w		(a2)+,d5	/* Read nametable data */
	add.w		a5,d5		/* Add tile number offset */
	or.w		#0x0000,d5	/* Set priority */
	move.w		d5,(a3)		/* Write to VRAM */
	subq.l		#1,a4
	cmp.l		#0,a4
	bgt 		_set_columns
	add.w		#0x80,d0
	add.l		d2,a2	
	subq.l		#1,counter2	/* Row counter */
	cmp.l		#0,counter2

	bgt 		_set_rows
	rts

#################################################
#        Load tile data from ROM                #
#                                               #
# Parameters:                                   #
#  a3: VRAM base                                # 
#  a4: pattern address                          #
#  d4: number of tiles to load                  #
#################################################

load_tiles:
	move.l 		#GFXCNTL,a2
	VRAM_ADDR_var 	d0,a3
	move.l 		d0,(a2)
	lsl		#3,d4
	
	move.l 		#GFXDATA,a3
	subq.l 		#1,d4
_copy_tile_data:
	move.l 		(a4)+,(a3)
	dbra 		d4,_copy_tile_data

	rts

#################################################
#  Load run length encoded tile data from ROM   #
#                                               #
# Parameters:                                   #
#  a3: VRAM base                                # 
#  a4: pattern address                          #
#  d4: number of tiles to load                  #
#################################################

load_tiles_rle:
	move.l 		#GFXCNTL,a2
	VRAM_ADDR_var 	d0,a3
	move.l 		d0,(a2)
	lsl		#5,d4
	
	move.l 		#GFXDATA,a3
	moveq.l		#0,d2	/* Shift count */
	moveq.l		#0,d3	/* Latch */
	moveq.l		#0,d0
_ltrle_1:
	move.b		(a4)+,d0
	move.l		d0,d1
	lsr.b		#4,d1	/* Run */
	and.b		#15,d0
_ltrle_2:
	move.l		d0,d5
	lsl.l		d2,d5
	or.l		d5,d3
	add.b		#4,d2
	cmp.b		#16,d2
	bne		_ltrle_3
	rol.w		#8,d3
	move.w		d3,(a3)
	subq.l		#2,d4
	moveq.l		#0,d3
	moveq.l		#0,d2
_ltrle_3:
	dbra		d1,_ltrle_2
	cmp		#0,d4
	bne		_ltrle_1
	_l_l:
	rts
	
#################################################
#        Clear one of the screen maps           #
#                                               #
# Parameters:                                   #
#  a0: Map address                              # 
#  d0: Data to write to each map entry          #
#################################################

clear_map:
	move.l 		#GFXCNTL,a4
	VRAM_ADDR_var	d1,a0
	move.l 		d1,(a4)
	move.l 		#GFXDATA,a3
	move.w		#1023,d1	/* Loop counter */
_clear_map_loop:
	move.w		d0,(a3)
	move.w		d0,(a3)
	dbra		d1,_clear_map_loop
	rts
	
#################################################
#        Load color data from ROM               #
#                                               #
# Parameters:                                   #
#  a3: CRAM base                                # 
#  a4: color list address                       #
#  d4: number of colors to load                 #
#################################################

load_colors:
	move.l 		#GFXCNTL,a2
	CRAM_ADDR_var 	d0,a3
	move.l 		d0,(a2)

	move.l 		#GFXDATA,a3
	subq.w		#1,d4
_copy_color_data:
	move.w		(a4)+,(a3)
	dbra		d4,_copy_color_data

	rts

#################################################
#       Wait for next VBlank interrupt          #
#################################################

wait_vsync:
	movea.l		#vtimer,a0
	move.l		(a0),a1
_wait_change:
	cmp.l		(a0),a1
	beq		_wait_change
	rts

#################################################
#                 ROM DATA                      #
#################################################

sprite_data:
  
Message:
	.ascii "HELLO WORLD"
Message_end:	
	dc.b 0

#################################################
#                 RAM DATA                      #
#################################################

.bss
.globl htimer
.globl vtimer
.globl rand_num
htimer:		.long 0
vtimer:		.long 0
rand_num:	.long 0
temp:		.long 0
framectr:	.long 0
counter1:	.long 0
counter2:	.long 0
spritean:	.long 0
scrollchar:	.long 0
textcolor:	.long 0
dtextc:		.long 0
counter3:	.long 0

sprite_data_ram:
	dc.l 0,0,0,0,0,0,0,0,0,0
	
.end
