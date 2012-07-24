; VARS.INC
; Defining where the variables are held in RAM

.enum ram
;needed
pal_from		ds 64	; Palette to fade FROM
game_clock		dw	; Tick-tock.					
cur_scroll_index	db	; How far the screen is scrolled							
rand_data		dw	; Random number tracker						
title_fade_text		db	; Fading in the TEXT counter					

.ende