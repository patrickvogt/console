;----------------------------------------------------------------
; Controller Screen Defines
;----------------------------------------------------------------

lb. ctrlr_char
	inc.		"img\ctrlr_char.inc"		; Controller charset
lb. ctrlr_map
	inc.		"img\ctrlr_map.inc"		; Controller map


; Define the OBJ's
lb.   CTLLeft			; Left on DPad
dh.	0x0000				; count
dh.	0x030D, 0x0000, 0x0000		; JCA, X, Y

lb.   CTLRight			; Right on DPad
dh.	0x0000				; count
dh.	0x030E, 0x0000, 0x0000	; JCA, X, Y

lb.   CTLUp				; Up on DPad
dh.	0x0000				; count
dh.	0x031D, 0x0000, 0x0000		; JCA, X, Y

lb.   CTLDown			; Down on DPad
dh.	0x0000				; count
dh.	0x031E, 0x0000, 0x0000		; JCA, X, Y

lb.   CTLButton1			; First Button
dh.	0x0003				; count
dh.	0x030F, 0x0000, 0x0000		; JCA, X, Y
dh.	0x0310, 0x0008, 0x0000		; JCA, X, Y
dh.	0x031F, 0x0000, 0x0008		; JCA, X, Y
dh.	0x0320, 0x0008, 0x0008		; JCA, X, Y

;----------------------------------------------------------------
; Controller Screen Helper Fn's
;----------------------------------------------------------------


;----------------------------------------------------------------
; Functions: LoadCtrlImg
;----------------------------------------------------------------
;  Load's the 2 controller immages into memory, for the 
;   controller demo
; Parameters:
; Dependency Regs:
;   $3,$4  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. LoadCtrlImg
	;Push return on stack
	add		-4,$29
	st.w		$31,0[$29]

;----------------------------------------------------------------
;  Reset OBJ Table
;----------------------------------------------------------------
	.mov32	VIP_START, $3		;Get a pointer to the VIP Reg
	st.h		$0, VIPR_SPT0[$3]		;SPT0
	st.h		$0, VIPR_SPT1[$3]		;SPT1
	st.h		$0, VIPR_SPT2[$3]		;SPT2
	st.h		$0, VIPR_SPT3[$3]		;SPT3

;----------------------------------------------------------------
;  Char Table
;----------------------------------------------------------------
	.mov32	ctrlr_char,   $3  	; char start in ram
	.mov32	CHR_TBL1,     $4		; char destination
	.mov32	0x00001000,   $5		; number of hwords to move
	.call		CopyHWord


;----------------------------------------------------------------
;  BGMap
;----------------------------------------------------------------
	;Storing our box in BGMAP0 can be done using the folowing asm code 
	.mov32	ctrlr_map, $3
	.mov32	BG_MAP,    $4		; char destination
	.mov32	0x00001000,$5		; number of hwords to move
	.call		CopyHWord

	;;Relocate BGMap
	movea		0x0000, $0, $3		;BGMap 0
	movea		0x0001, $0, $4		;CharTable 1
	.call		movBGMap

;----------------------------------------------------------------
;  World Map
;----------------------------------------------------------------
	; $5 is our variable value

	;-- Left Controller --
	.mov32	0x0003DBE0, $1		; World 31 

	.mov32	0x00008000, $5		;Set Lon, Ron, BGMap Base
	st.h		$5, 0[$1]			;Store value

	movea		0x007F, $0, $5		;GX = 127 (Screen)
	st.h		$5, 2[$1]
	mov		0, $5				;GP = 0
	st.h		$5, 4[$1]			;A Simple 3D Effect

	movea		0x0038, $0, $5		;GY = 56 (Screen)
	st.h		$5, 6[$1]

	movea		0x0000, $0, $5		;MX = 0 (Screen)
	st.h		$5, 8[$1]
	mov		0, $5				;MP = 0
	st.h		$5, 10[$1]

	movea		0x0000, $0, $5		;MY = 0 (Screen)
	st.h		$5, 12[$1]

	movea		0x007F, $0, $5		;W = 127
	st.h		$5, 14[$1] 
	movea		0x006F, $0, $5		;H = 111
	st.h		$5, 16[$1]

	st.h		$0, 18[$1]			;Param Base
	st.h		$0, 20[$1]			;Overplane Char

	;-- Right Controller --
	addi		0xFFE0, $1, $1		;World 30, subtract 0x20

	movea		0x4000, $0, $5		;Set Lon, Ron, BGMap Base
	st.h		$5, 0[$1]			;Store value

	movea		0x007F, $0, $5		;GX = 127 (Screen)
	st.h		$5, 2[$1]
	mov		0, $5				;GP = 0
	st.h		$5, 4[$1]			;A Simple 3D Effect

	movea		0x0038, $0, $5		;GY = 56 (Screen)
	st.h		$5, 6[$1]

	movea		0x0080, $0, $5		;MX = 128 (BG_MAP)
	st.h		$5, 8[$1]
	mov		0, $5				;MP = 0
	st.h		$5, 10[$1]
	xor		$5, $5
	movea		0x0000, $0, $5		;MY = 0 (BG_MAP)
	st.h		$5, 12[$1]

	movea		0x007F, $0, $5		;W = 127
	st.h		$5, 14[$1] 
	movea		0x006F, $0, $5		;H = 111
	st.h		$5, 16[$1]

	xor		$5, $5
	st.h		$5, 18[$1]			;Param Base
	st.h		$5, 20[$1]			;Overplane Char

	;-- Nintendo Logo --
	addi		0xFFE0, $1, $1		;World 29, subtract 0x20

	.mov32	0xC000, $5			;Set Lon, Ron, BGMap Base
	st.h		$5, 0[$1]			;Store value

	movea		0x00A0, $0, $5		;GX = 160 (Screen)
	st.h		$5, 2[$1]
	mov		6, $5				;GP = 6
	st.h		$5, 4[$1]			;A Simple 3D Effect

	movea		0x00C8, $0, $5		;GY = 200 (Screen)
	st.h		$5, 6[$1]

	movea		0x0080, $0, $5		;MX = 128 (BGMap)
	st.h		$5, 8[$1]
	mov		0, $5				;MP = 0
	st.h		$5, 10[$1]

	movea		0x0078, $0, $5		;MY = 120 (BGMap)
	st.h		$5, 12[$1]

	movea		0x003F, $0, $5		;W = 63
	st.h		$5, 14[$1] 
	movea		0x000F, $0, $5		;H = 15
	st.h		$5, 16[$1]

	st.h		$0, 18[$1]			;Param Base
	st.h		$0, 20[$1]			;Overplane Char

	;-- World Obj, for keys --
	addi		0xFFE0, $1, $1		;World 28, subtract 0x20
	.mov32	0x0000F000, $5		;Set Lon, Ron, BGMap Base
	st.h		$5, 0[$1]			;Store value

	st.h		$0, 2[$1]			;GX = 0 (Screen)
	st.h		$0, 4[$1]			;GP = 0
	st.h		$0, 6[$1]			;GY = 0 (Screen)
	st.h		$0, 8[$1]			;MX = 0 (Screen)
	st.h		$0, 10[$1]			;MP = 0
	st.h		$0, 12[$1]			;MY = 0 (Screen)

	movea		0x01FF, $0, $5
	st.h		$5, 14[$1] 			;W = 511
	st.h		$5, 16[$1]			;H = 511

	st.h		$0, 18[$1]			;Param Base
	st.h		$0, 20[$1]			;Overplane Char


	;-- End World --
	addi		0xFFE0, $1, $1		;World 27, subtract 0x20
	movea		0x40, $0, $5		;0x0040...End
	st.h		$5, 0[$1]			;Load World Desc Part

	;POP return from stack
	ld.w		0[$29],$31
	add		4,$29

	jmp 		[$31]				; return


;----------------------------------------------------------------
; Functions: RunCtrlImg
;----------------------------------------------------------------
;  Reads the controller, and displays the buttons
;  for the controller demo
; Parameters:
; Dependency Regs:
;   $3,$4,$5,$6,$7,$20,$21  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. RunCtrlImg
	;Push return on stack
	add		-4,$29
	st.w		$31,0[$29]


	.call		ReadPad			; Clear the controller buffer
	mov		$3, $20			; Save results to another register

	;; Pointer to VIP mem
	movhi		0x0006, $0, $3		;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register
	;Remove old sprites
	
	mov		$0, $7			;zero Sprite pointer
	st.h		$7, VIPR_SPT3[$3]		;Set SPT3 to 0

  ;-- Left/Right D-Pad --
	;store paralax once, to save space
	movea		0xC001, $0, $6		;JP

	.mov32	CTLLeft, $3			;Pointer to map
	; Left on L-DPad
	andi		0x0200, $20, $21		;Left on L-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x008B, $0, $4		;JX
	movea		0x0041, $0, $5		;JY
	.call		addObj
	; Left on R-DPad
	andi		0x4000, $20, $21		;Left on R-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00D8, $0, $4		;JX
	movea		0x0041, $0, $5		;JY
	.call		addObj

	.mov32	CTLRight, $3		;Pointer to map
	; Right on L-DPad
	andi		0x0100, $20, $21		;Right on L-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x009E, $0, $4		;JX
	movea		0x0041, $0, $5		;JY
	.call		addObj
	; Right on R-DPad
	andi		0x0080, $20, $21		;Right on R-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00EB, $0, $4		;JX
	movea		0x0041, $0, $5		;JY
	.call		addObj

	.mov32	CTLUp, $3			;Pointer to map
	; Up on L-DPad
	andi		0x0800, $20, $21		;Up on L-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x0095, $0, $4		;JX
	movea		0x0038, $0, $5		;JY
	.call		addObj
	; Up on R-DPad
	andi		0x0040, $20, $21		;Up on R-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00E2, $0, $4		;JX
	movea		0x0038, $0, $5		;JY
	.call		addObj

	.mov32	CTLDown, $3			;Pointer to map
	; Down on L-DPad
	andi		0x0400, $20, $21		;Down on L-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x0095, $0, $4		;JX
	movea		0x004B, $0, $5		;JY
	.call		addObj
	; Down on R-DPad
	andi		0x8000, $20, $21		;Down on R-DPad, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00E2, $0, $4		;JX
	movea		0x004B, $0, $5		;JY
	.call		addObj

  ;-- Buttons --

	;store paralax once, to save space
	movea		0xC000, $0, $6		;JP
	.mov32	CTLButton1, $3		;Pointer to map

	;A
	andi		0x0004, $20, $21		;A, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00D0, $0, $4		;JX
	movea		0x0044, $0, $5		;JY
	.call		addObj

	;B
	andi		0x0008, $20, $21		;B, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00C5, $0, $4		;JX
	movea		0x0049, $0, $5		;JY
	.call		addObj

	;R
	andi		0x0010, $20, $21		;R, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00DA, $0, $4		;JX
	movea		0x0029, $0, $5		;JY
	.call		addObj

	;L
	andi		0x0020, $20, $21		;L, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x0090, $0, $4		;JX
	movea		0x0029, $0, $5		;JY
	.call		addObj

	;Start
	andi		0x1000, $20, $21		;Start, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x00A7, $0, $4		;JX
	movea		0x0049, $0, $5		;JY
	.call		addObj

	;Select
	andi		0x2000, $20, $21		;Select, dont hurt the origional data
	be		0x1C				;If not pressed go to next
	movea		0x009C, $0, $4		;JX
	movea		0x0044, $0, $5		;JY
	.call		addObj


	;POP return from stack
	ld.w		0[$29],$31
	add		4,$29

	jmp 		[$31]				; return
