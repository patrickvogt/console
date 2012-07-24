;----------------------------------------------------------------
; Focus Screen Defines
;----------------------------------------------------------------

lb. default_char
	inc.		"img\default_char.inc"		; Standard CharSet
lb. focus_map
	inc.		"img\focus_map.inc"		; Description of focus map

;----------------------------------------------------------------
; Focus Screen Helper Fn's
;----------------------------------------------------------------


;----------------------------------------------------------------
; Load the base immage
;----------------------------------------------------------------
lb. LoadFocusImg
	;Push return on stack
	add		-4,$29
	st.w		$31, 0[$29]
;----------------------------------------------------------------
;  Char Table
;----------------------------------------------------------------
	.mov32	default_char, $3  	; char start in ram
	.mov32	CHR_TBL0,     $4		; char destination
	.mov32	0x00000FFF,   $5		; number of hwords to move
	.call		CopyHWord


;----------------------------------------------------------------
;  BGMap
;----------------------------------------------------------------
	;Storing our box in BGMAP0 can be done using the folowing asm code 
	.mov32	Focus_Map, $3
	.mov32	BG_MAP,    $4		; char destination
	.mov32	0x00000FFF,$5		; number of hwords to move
	.call		CopyHWord

;----------------------------------------------------------------
;  World Map
;----------------------------------------------------------------
	; $5 is our variable value

	; Left Focus screen
	.mov32	0x0003DBE0, $1		; World 31 

	.mov32	0x00008000, $5, $5	;Set Lon, Ron, BGMap Base
	st.h		$5, 0[$1]			;Store value

	st.h		$5, 0[$1]			;Store value
	movea		0, $0, $5			;GX = 0 (Screen)
	st.h		$5, 2[$1]
	mov		0, $5				;GP = 0
	st.h		$5, 4[$1]			;A Simple 3D Effect
	movea		0, $0, $5			;GY = 0 (Screen)
	st.h		$5, 6[$1]

	movea		0, $0, $5			;MX = 0 (BG_MAP)
	st.h		$5, 8[$1]
	mov		0, $5				;MP = 0
	st.h		$5, 10[$1]
	movea		0, $0, $5			;MY = 0 (BG_MAP)
	st.h		$5, 12[$1]

	.mov32	384, $5			;W = 384
	st.h		$5, 14[$1] 
	.mov32	224, $5			;H = 224
	st.h		$5, 16[$1]

	xor		$5, $5
	st.h		$5, 18[$1]			;Param Base
	st.h		$5, 20[$1]			;Overplane Char

	; Right Focus screen
	.mov32	0x0003DBC0, $1		;World 30

	.mov32	0x00004000, $5, $5	;Set Lon, Ron, BGMap Base
	st.h		$5, 0[$1]			;Store value

	movea		0, $0, $5			;GX = 0 (Screen)
	st.h		$5, 2[$1]
	mov		0, $5				;GP = 0
	st.h		$5, 4[$1]			;A Simple 3D Effect
	movea		0, $0, $5			;GY = 0 (Screen)
	st.h		$5, 6[$1]

	movea		0, $0, $5			;MX = 00 (Screen)
	st.h		$5, 8[$1]
	mov		0, $5				;MP = 0
	st.h		$5, 10[$1]
	xor		$5, $5
	.mov32	224, $5			;MY = 224 (Screen)
	st.h		$5, 12[$1]

	.mov32	384, $5			;W = 384
	st.h		$5, 14[$1] 
	.mov32	224, $5			;H = 224
	st.h		$5, 16[$1]

	xor		$5, $5
	st.h		$5, 18[$1]			;Param Base
	st.h		$5, 20[$1]			;Overplane Char

	; End World
	.mov32	0x0003DBA0, $1		;World 29
	movea		0x40, $0, $5		;0x0040...End
	st.h		$5, 0[$1]			;Load World Desc Part

	;POP return from stack
	ld.w		0[$29], $31
	add		4,$29

	jmp 		[$31]			; return
