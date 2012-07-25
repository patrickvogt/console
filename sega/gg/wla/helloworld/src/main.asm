;==============================================================
; WLA-DX DIRECTIVES
;==============================================================

.memorymap 
defaultslot 0 
slotsize $7ff0 
slot 0 $0000 
slotsize $10 
slot 1 $7ff0 
slotsize $4000 
slot 2 $8000 
.endme 

.rombankmap 
bankstotal 4
banksize $7ff0 
banks 1 
banksize $10 
banks 1 
banksize $4000 
banks 2 
.endro

.sdsctag 1.0,"Fire Track","Space Shooter","Ben Ryves"
.bank 0 slot 0

.define ENABLE_TITLES			1; Display intro titles?

;==============================================================
; GENERAL HEADER FILES
;==============================================================

.include "gamegear.inc"
.include "vars.inc"

;==============================================================
; BOOT
;==============================================================

.org $0000

	di
	im 1

	jp main

;==============================================================
; INTERRUPT HANDER
;==============================================================

.org $0038
	ret

;==============================================================
; PAUSE HANDLER [SMS, UNUSED]
;==============================================================

.org $0066
	retn
  
;==============================================================
; EXTERNAL SOURCE FILES
;==============================================================

.include "routines.asm"		; General Game Gear routines
.include "misc.asm"		; General Fire Track routines

;==============================================================
; EXTERNAL DATA FILES
;==============================================================

font:
.incbin "../bin/pattern_table_1.chr"
fontend:

;==============================================================
; MAIN PROGRAM ENTRY POINT
;==============================================================

main:
;==============================================================
; INITIALISE THE RAW BASICS
;==============================================================

	xor a 
	ld ($FFFC),a 
	ld ($FFFD),a 
	inc a 
	ld ($FFFE),a 
	inc a 
	ld ($FFFF),a

	ld sp,$DF00
   	call vid_reset

	ld a,%00001110
	ld b,vdp_reg_overscan
	call vid_set_vdp_reg

	call vid_clear_vram

	in a,(joystick)
	and joy_1|joy_2

;==============================================================
; RUN THE TITLE SCREEN[S]
;==============================================================

.ifdef ENABLE_TITLES
.include "titlepic.asm"		; Snazzy title image
.include "title.asm"		; Pink and red job
.endif

	call vid_clear_vram

;==============================================================
; SET UP INTERRUPTS
;==============================================================

	ld a,%00011100
	ld b,vdp_reg_mode_1
	call vid_set_vdp_reg
	ei
	
;==============================================================
; MAIN GAME LOOP
;==============================================================

main_loop:
	ei

	ld hl,(game_clock)
	inc hl
	ld (game_clock),hl
	
;==============================================================
; JUMP BACK TO MAIN GAME LOOP
;==============================================================

	jp main_loop

.slot 2
.section "Title picture data" superfree
.include "titlepic.inc"
.include "title.inc"		; Title screen
.ends