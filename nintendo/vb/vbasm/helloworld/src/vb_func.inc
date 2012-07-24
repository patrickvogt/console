;----------------------------------------------------------------
; Functions: SetColTbl
;----------------------------------------------------------------
;  Initializes the Column Table - called first thing, at the begining of your program
; Parameters:
; Dependency Regs:
;   $3,$4  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------

; Defines used by Fn
lb. ColTbl
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
db. 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xe0, 0xbc
db. 0xa6, 0x96, 0x8a, 0x82, 0x7a, 0x74, 0x6e, 0x6a
db. 0x66, 0x62, 0x60, 0x5c, 0x5a, 0x58, 0x56, 0x54
db. 0x52, 0x50, 0x50, 0x4e, 0x4c, 0x4c, 0x4a, 0x4a
db. 0x48, 0x48, 0x46, 0x46, 0x46, 0x44, 0x44, 0x44
db. 0x42, 0x42, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40
db. 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3c
db. 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c
db. 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c
lb. ColTblE

;----------------------------------------------------------------
lb. SetColTbl
	;Write first half of column table
	movhi		0x0004, $0, $3		;0x0003DC00 => $3
	movea		0xDC00, $3, $3		;Start of column table
	movhi		0x0004, $0, $4		;0x0003DCFF => $3
	movea		0xDCFF, $4, $4		;End of column table
	.mov32	ColTbl, $6		; Column Table
 lb. CTBL_Loop1
	ld.b		0[$6], $5		; Data to write
	shl		24, $5		; zero high bits
	shr		24, $5
	st.h		$5, 0x0000[$3]	;Column Table 1
	st.h		$5, 0x0200[$3]	;Column Table 2

	add		2, $3
	add		1, $6

	cmp		$4,$3
	bge		12
	.jump 	CTBL_Loop1		

	;Write second half of column table
	movhi		0x0004, $0, $3		;0x0003DDC00 => $3
	movea		0xDD00, $3, $3		;Start of column table
	movhi		0x0004, $0, $4		;0x0003DDCFF => $3
	movea		0xDDFF, $4, $4		;End of column table
	.mov32	ColTbl, $6		; Column Table
	addi		0x7F,$6,$6
 lb. CTBL_Loop2
	ld.b		0[$6], $5		; Data to write
	shl		24, $5
	shr		24, $5
	st.h		$5, 0x0000[$3]	;Column Table 1
	st.h		$5, 0x0200[$3]	;Column Table 2

	add		2, $3
	add		-1, $6

	cmp		$4,$3
	bge		12
	.jump 	CTBL_Loop2		


	jmp 		[$31]			; return


;----------------------------------------------------------------
; Functions: ReadPad
;----------------------------------------------------------------
; Parameters:
;   $3     - Read value (HWord, Return)
; Dependency Regs:
;   $4     - Destination start in rom
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. ReadPad
	;; Pointer to Hardware Controll Reg
	movhi		0x200, $0, $4		;0x02000000 => $4
	xor		$2,$2
	movea		0x0080, $0, $2	
	st.b		$2, HCREGR_KCR[$4]	;Tell Controller to reset
	add		0x04, $2	
	st.b		$2, HCREGR_KCR[$4]	;Tell Controller to read

	ld.b		HCREGR_KCR[$4], $2	;Read status
	andi		0x0002, $2, $2
	bne		-8				;Wait if not done

	ld.b		HCREGR_KHB[$4], $2	;Read Upper Byte
	shl		24, $2			;remove sign bit
	shr		16, $2
	ld.b		HCREGR_KLB[$4], $3	;Read Lower Byte
	shl		24, $3			;remove sign bit
	shr		24, $3

	or		$2, $3			;put them together in $3
	jmp		[$31]				;return


;----------------------------------------------------------------
; Functions: CopyByte
;----------------------------------------------------------------
; Parameters:
;   $3     - Source start in rom
;   $4     - Destination start in rom
;   $5     - # of bytes to copy
; Dependency Regs:
;   $6     - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. CopyByte
	ld.b		0[$3],$6		; copy our data
	st.b		$6, 0[$4]
	add		1, $3			; increment our Source/Dest pointers
	add		1, $4
	add		-1, $5		; Decrement our counter
	be		12			; if Zero, exit
	.jump		CopyByte		; else loop to start
	jmp 		[$31]			; return

;----------------------------------------------------------------
; Functions: CopyHWord
;----------------------------------------------------------------
; Parameters:
;   $3     - Source start in rom
;   $4     - Destination start in rom
;   $5     - # of HWords to copy
; Dependency Regs:
;   $6     - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. CopyHWord
	ld.h		0[$3],$6		; copy our data
	st.h		$6, 0[$4]
	add		2, $3			; increment our Source/Dest pointers
	add		2, $4
	add		-1, $5		; Decrement our counter
	be		12			; if Zero, exit
	.jump		CopyHWord		; else loop to start
	jmp 		[$31]			; return


;----------------------------------------------------------------
; Functions: CopyWord
;----------------------------------------------------------------
; Parameters:
;   $3     - Source start in rom
;   $4     - Destination start in rom
;   $5     - # of Words to copy
; Dependency Regs:
;   $6     - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. CopyWord
	ld.w		0[$3],$6		; copy our data
	st.w		$6, 0[$4]
	add		4, $3			; increment our Source/Dest pointers
	add		4, $4
	add		-1, $5		; Decrement our counter
	be		12			; if Zero, exit
	.jump		CopyWord		; else loop to start
	jmp 		[$31]			; return


;----------------------------------------------------------------
; Functions: VBReset
;----------------------------------------------------------------
;  Initializes the VB - called at the begining of your program
; Parameters:
; Dependency Regs:
; $3,$4,$5,$28  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. VBReset
	;-- Zero out Program memory 0x05000000-0x0500FFFF --
	;; mem end
	movhi		0x501, $0, $3		;0x05010000 => $3
	;; mem pointer
	movhi		0x500, $0, $4		;0x05000000 => $4
	jr		0x0A				; jump to loop start
	;; loop top
	st.h		$0, 0[$4]			;Zero Memory
	add		2, $4				;Increment mem pointer
	;; loop start
	cmp		$3, $4			;End of memory?
	blt		-8				; if not goto loop top

	;-----------------------------------
	; Init VIP
	;-----------------------------------
	;-- Set World 31 to off with 'ovr' bit on (Write 0x0040 to 0x0003DBE0) --
	;; set $4 to 0x00-0x1F to select world...
	movea		0x1F, $0, $4		;0x001F => $4
	shl		5, $4				;0x03E0 => $4
	movhi		4, $0, $3			;0x00040000 => $3
	movea		0xD800, $3, $5		;0x0003D800 => $5
	add		$4, $5			;0x0003DBE0 => $5
	;; value to clear world to
	movea		0x40, $0, $4		;0x00000040 => $4
	st.h		$4, 0[$5]			;World n (31), OVR=1

	;-- Zero VIP Mem 0x00000000-0x0001FFFF (char mem) --
	;; Mem end
	movhi		2, $0, $3			;0x0001FFFF => $3
	movea		0xFFFF, $3, $3
	;; Mem pointer
	mov		$0, $4			;0x00000000 => $4
	jr		0x0A				;jump to loop start
	;; loop top
	st.h		$0, 0[$4]			;Zero VIP Memory
	add		2, $4				;increment mem pointer
	;; loop start
	cmp		$3, $4			;end of Memory?
	blt		-8				;if not goto loop top

	;-- Read DPSTTS(0x0005F820), and with 0xFFFF, or with 0x0101 --
	;--  and write to DPCTRL(0x0005F826) --
	;; Pointer to VIP mem
	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register

	;; Stor DPSTTS in $4
	ld.h		VIPR_DPSTTS[$3], $4		;[0x0005F820] => $4
	andi		0xffff, $4, $4		;Zero High HWord (why?)
	ori		0x0101, $4, $4		;Turn on bit 0 and 8
	;; Store $4 in DPCTRL
	st.h		$4, VIPR_DPCTRL[$3]		;$4 => [0x0005F822]

	;-- Write 0x0000 to INTENB (0x0005F802) --
	st.h		$0, VIPR_INTENB[$3]	;INTENB
	;-- Write 0xE01F to INTCLR (0x0005F804) --
	movea		0xE01F, $0, $4
	st.h		$4, VIPR_INTCLR[$3]	;INTCLR
	;-- Write 0x0001 to XPCTRL (0x0005F842) --
	;-- Write 0x0001 to FRMCYC (0x0005F82E) --
	movea		0x0001, $0, $4
	st.h		$4, VIPR_XPCTRL[$3]	;XPCTRL
	st.h		$4, VIPR_FRMCYC[$3]	;FRMCYC

	;  -- Write 0x0000 to REST   (0x0005F82A) --
	st.h		$0, VIPR_REST[$3]		;REST

	;-- Set Display Pallet Info --
	;  -- Write 0x000C to BRTA   (0x0005F824) --
	;  -- Write 0x0028 to BRTB   (0x0005F826) --
	;  -- Write 0x0030 to BRTC   (0x0005F828) --
	movea		0x0C, $0, $4		;Load the Color Intensity
	st.h		$4, VIPR_BRTA[$3]		;BRTA
	addi		0x1C,$4, $4
	st.h		$4, VIPR_BRTB[$3]		;BRTB
	add		0x8,$4
	st.h		$4, VIPR_BRTC[$3]		;BRTC

	;  -- Write 0x00E4 to GPLT0   (0x0005F860) --
	;  -- Write 0x00E4 to GPLT1   (0x0005F862) --
	;  -- Write 0x00E4 to GPLT2   (0x0005F864) --
	;  -- Write 0x00E4 to GPLT3   (0x0005F866) --
	;  -- Write 0x00E4 to JPLT0   (0x0005F868) --
	;  -- Write 0x00E4 to JPLT1   (0x0005F86A) --
	;  -- Write 0x00E4 to JPLT2   (0x0005F86C) --
	;  -- Write 0x00E4 to JPLT3   (0x0005F86E) --
	movea		0xE4, $0, $4		;Pallette Value: 11100100b
	st.h		$4, VIPR_GPLT0[$3]	;GPLT0
	st.h		$4, VIPR_GPLT1[$3]	;GPLT1
	st.h		$4, VIPR_GPLT2[$3]	;GPLT2
	st.h		$4, VIPR_GPLT3[$3]	;GPLT3
	st.h		$4, VIPR_JPLT0[$3]	;JPLT0
	st.h		$4, VIPR_JPLT1[$3]	;JPLT1
	st.h		$4, VIPR_JPLT2[$3]	;JPLT2
	st.h		$4, VIPR_JPLT3[$3]	;JPLT3

	;  -- Write 0x0000 to BGCOL   (0x0005F870) --
	st.h		$0, VIPR_BKCOL[$3]	;BKCOL

	;-- Clear SPTx pointers to 0 --
	;  -- Write 0x0000 to SPT3    (0x0005F84E) --
	;  -- Write 0x0000 to SPT2    (0x0005F84C) --
	;  -- Write 0x0000 to SPT1    (0x0005F84A) --
	;  -- Write 0x0000 to SPT0    (0x0005F848) --
	st.h		$0, VIPR_SPT3[$3]		;SPT3
	st.h		$0, VIPR_SPT2[$3]		;SPT2
	st.h		$0, VIPR_SPT1[$3]		;SPT1
	st.h		$0, VIPR_SPT0[$3]		;SPT0

	;-- Set World 31 to off with 'ovr' bit on (Write 0x0040 to 0x0003DBE0) --
	;(repeat again, is this nesisary?)
	;; set $4 to 0x00-0x1F to select world...
	movea		0x1F, $0, $4		;0x001F => $4
	shl		5, $4				;0x03E0 => $4
	movhi		4, $0, $3			;0x00040000 => $3
	movea		0xD800, $3, $5		;0x0003D800 => $5
	add		$4, $5			;0x0003DBE0 => $5
	;; value to clear world to
	movea		0x40, $0, $4		;0x00000040 => $4
	st.h		$4, 0[$5]			;World n (31), OVR=1

	;-- Zero VIP Mem 0x00000000-0x0001FFFF (char mem) --
	;(repeat again, is this nesisary?)
	;; Mem end
	movhi		2, $0, $3			;0x0001FFFF => $3
	movea		0xFFFF, $3, $3
	;; Mem pointer
	mov		$0, $4			;0x00000000 => $4
	jr		0x0A				;jump to loop start
	;; loop top
	st.h		$0, 0[$4]			;Zero VIP Memory
	add		2, $4				;increment mem pointer
	;; loop start
	cmp		$3, $4			;end of Memory?
	blt		-8				;if not goto loop top
	
	;-- Load Column Tables --
	mov		$31, $28			;Save return address
	.call		SetColTbl			; Initialize the column table
	mov		$28, $31			;Restore return address

;-- Read DPSTTS until bit 6 is set --
	;; Pointer to VIP mem
	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register
	;; loop top
	ld.h		VIPR_DPSTTS[$3], $4	;load DPSTTS in $4
	andi		0xffff, $4, $4		;look at lower 16 bytes
	andi		0x40, $4, $4		;is bit 6 on?
	be		-12				; if not goto loop top

	;-----------------------------------
	; Init Processor
	;-----------------------------------
	;-- Set EIPSW to 0 and CHCW to 0x01 --
	mov		$0, $5			;0x00000000 => $5
	ldsr		$5, $5			;0x00000000 => S$5  ????
	mov		2, $5				;0x00000002 => $5
	ldsr		$5, $24			;0x00000002 => S$24 ????

	;-----------------------------------
	; Init HCReg 
	;-----------------------------------
	;-- Init Link Port (nesisary?) --
	;; Pointer to Hardware Controll Reg
	movhi		0x200, $0, $3		;0x02000000 => $3
	;; Write 0x80 to LPT (data to transmit)
	movea		0xff80, $0, $4		;0xFFFFFF80 => $4
	st.b		$4, HCREGR_LPT[$3]	;0x80 => [0x02000008]
	;; Write 0x14 to LPC (ComStart, External Clock)
	movea		0x14, $0, $4		;0x00000014 => $4
	st.b		$4, HCREGR_LPC[$3]	;0x14 => [0x02000000]
	;; Write 0xFF to LPC2 (Disable Interupt, etc...)
	mov		-1, $4			;0xFFFFFFFF => $4
	st.b		$4, HCREGR_LPC2[$3]	;0xFF => [0x02000004]

	;-- Write 0x00 to TCR (Disable Timmer) --
	;; Write 0 to TCR (timmer disabled)
	st.b		$0, HCREGR_TCR[$3]	;0x00 => [0x02000020]

	;-- Write 0x80 to KCR (Disable Keypad Interupts) --
	movea		0x0080, $0, $4	
	st.b		$4, HCREGR_KCR[$3]	;Tell Controller to reset, disable interupts

	;-----------------------------------
	; Init Sound, (nesisary?)
	;-----------------------------------
	;-- Set SMREG to 0x01 --
	;; Pointer to Sound Mem
	movhi		0x100, $0, $5		;0x01000000 => $5
	;; Write 0x01 to SMREG
	mov		1, $3				;0x00000001 => $3
	st.b		$3, SCREGR_SMRG[$5]	;0x01 => [0x01000580]

	;-- Set SxCTRL and SxLEN to 0 for all 6 Sound chanels --
	mov		$0, $3			;0x00000000 => $3
	jr		22				;Jump to loop start
	;; Loop top
	mov		$3, $4			;Copy $3 to $4
	;; $4 is pointer to Sx where x = 0-5
	shl		6, $4				;Multiply by 128?
	movhi		0x100, $4, $5		;Add 0x01000000 to $4
	st.b		$0, 0x404[$5]		;Set SxLEN to 0x00
	st.b		$0, 0x400[$5]		;Set SxCTRL to 0x00
	add		1, $3				;Increment mem pointer
	;; Loop start
	cmp		6, $3				;end of loop?
	blt		-20				;if not goto 0x0700B04C

	;-- Delay for 0x2000 cycles --
	mov		$0, $3			;0x00000000 => $3
	movea		0x2000, $0, $5		;0x00002000 => $5
	jr		6				;Jump to LoopStart
	;; loop top
	add		1, $3				;increment counter
	;; loop start
	cmp		$5, $3			;end of loop?
	blt		-4				;if not goto LoopTop

	;-- Fill in Sound Data Ram for all 6 chanels --
; (fill in)

	;-- Set SMREG to 0x00 --
	;; Pointer to Sound Mem
	movhi		0x100, $0, $5		;0x01000000 => $5
	;; Write 0x01 to SMREG
	st.b		$0, SCREGR_SMRG[$5]	;0x00 => [0x01000580]


	;-------------------------
	; Set Display up for writes to Char & BGMap mem
	;-------------------------
	;; Pointer to VIP mem
	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register

	movea		0x0001, $0, $4
	st.h		$4, VIPR_XPCTRL[$3]	;XPCTRL
	movea		0x0101, $0, $4
	st.h		$4, VIPR_DPCTRL[$3]	;DPCTRL

	;Initialize our stack
	movhi		0x0501, $0, $29		;0x0500FFFF => $29
	movea		0xFFFF, $29, $29
	
	jmp 		[$31]				; return



;----------------------------------------------------------------
; Functions: DspOn
;----------------------------------------------------------------
;  Turns the display on
; Parameters:
; Dependency Regs:
;   $3,$4  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. DspOn
	; Display On

	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register

	;Reset 15 minut rest timmer
	st.h		$0, VIPR_REST[$3]		;Rest

	;Turn on display?
	ld.h		VIPR_XPSTTS[$3], $4 	;XPSTTS
	andi		0xFFFF, $4, $4
	ori		0x0002, $4, $4
	st.h		$4, VIPR_XPCTRL[$3]	;XPCtrl

	;Turn on display
	ld.h		VIPR_DPSTTS[$3], $4 	;DPSTTS
	andi		0xFFFF, $4, $4
	ori		0x0302, $4, $4
	st.h		$4, VIPR_DPCTRL[$3]	;DPCtrl

	;reset refresh rate?
	st.h		$0, VIPR_FRMCYC[$3]	;FRMCYC

	;clear out extranious interupts
	ld.h		VIPR_INTPND[$3], $4 	;INTPND
	st.h		$4, VIPR_INTCLR[$3]	;INTCLR

	;Wait for retrace?
	;loop top
	;read DPSTTS into $4
	ld.h		VIPR_DPSTTS[$3], $4	;[0x0005F820] => $1
	andi		0x3C, $4, $4		;mask with 0x3C
	;andi		0x40, $4, $4		;mask with 0x40
	be		-8				;while != 0x3C goto 0x0700019A

	jmp 		[$31]				; return

;----------------------------------------------------------------
; Functions: DspOff
;----------------------------------------------------------------
;  Turns the display off
; Parameters:
; Dependency Regs:
;   $3,$4  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. DspOff
	; Display Off
	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register

	;Reset 15 minut rest timmer
	st.h		$0, VIPR_REST[$3]		;Rest

	;Wait for retrace?
	;loop top
	;read DPSTTS into $4
	ld.h		VIPR_DPSTTS[$3], $4	;[0x0005F820] => $1
	andi		0x3C, $4, $4		;mask with 0x3C
	;andi		0x40, $4, $4		;mask with 0x40
	be		-8				;while != 0x3C goto 0x0700019A

	movea		0x0001, $0, $4		;Display off
	st.h		$4, VIPR_XPCTRL[$3]	;XPCtrl
	movea		0x0101, $0, $4
	st.h		$4, VIPR_DPCTRL[$3]	;DPCTRL
	movea		0x0001, $0, $4
	st.h		$4, VIPR_FRMCYC[$3]	;FRMCYC
	jmp 		[$31]				; return

;----------------------------------------------------------------
; Functions: DspPalOn
;----------------------------------------------------------------
;  Turns the pallet britness up
; Parameters:
; Dependency Regs:
;   $3,$4  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. DspPalOn
	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register

	movea		0x0C, $0, $4		;Load the Color Intensity
	st.h		$4, VIPR_BRTA[$3]		;BRTA
	addi		0x1C,$4, $4
	st.h		$4, VIPR_BRTB[$3]		;BRTB
	add		0x8,$4
	st.h		$4, VIPR_BRTC[$3]		;BRTC

	; remove me
	movea		0xE4, $0, $4		;Pallette Value: 11100100b
	st.h		$4, VIPR_GPLT0[$3]	;GPLT0
	st.h		$4, VIPR_GPLT1[$3]	;GPLT1
	st.h		$4, VIPR_GPLT2[$3]	;GPLT2
	st.h		$4, VIPR_GPLT3[$3]	;GPLT3
	st.h		$4, VIPR_JPLT0[$3]	;JPLT0
	st.h		$4, VIPR_JPLT1[$3]	;JPLT1
	st.h		$4, VIPR_JPLT2[$3]	;JPLT2
	st.h		$4, VIPR_JPLT3[$3]	;JPLT3
	st.h		$0, VIPR_BKCOL[$3]	;BKCOL

	jmp 		[$31]				; return

;----------------------------------------------------------------
; Functions: DspPalOff
;----------------------------------------------------------------
;  Turns the pallet britness off
; Parameters:
; Dependency Regs:
;   $3,$4  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. DspPalOff
	; Pallet On
	movhi		6, $0, $3			;0x0005F800 => $3
	movea		0xf800, $3, $3		;pointer to VIP Register

	movea		0x00, $0, $4		;Load the Color Intensity
	st.h		$4, VIPR_BRTA[$3]		;BRTA
	st.h		$4, VIPR_BRTB[$3]		;BRTB
	st.h		$4, VIPR_BRTC[$3]		;BRTC

	; remove me
	movea		0x00, $0, $4		;Pallette Value: 00000000b
	st.h		$4, VIPR_GPLT0[$3]	;GPLT0
	st.h		$4, VIPR_GPLT1[$3]	;GPLT1
	st.h		$4, VIPR_GPLT2[$3]	;GPLT2
	st.h		$4, VIPR_GPLT3[$3]	;GPLT3
	st.h		$4, VIPR_JPLT0[$3]	;JPLT0
	st.h		$4, VIPR_JPLT1[$3]	;JPLT1
	st.h		$4, VIPR_JPLT2[$3]	;JPLT2
	st.h		$4, VIPR_JPLT3[$3]	;JPLT3
	st.h		$0, VIPR_BKCOL[$3]	;BKCOL
	nop

	jmp 		[$31]				; return


;----------------------------------------------------------------
; Functions: DelayXPSTTS
;----------------------------------------------------------------
;  Delays based on the XPStatus screen changed Bit
; Parameters:
; Dependency Regs:
;   $3     - Delay count
;   $4,$5,$6  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. DelayXPSTTS
	movhi		6, $0, $4			;0x0005F800 => $4
	movea		0xf800, $4, $4		;pointer to VIP Register

lb. XPSTTS_Loop1					; Loop n times
	ld.h		VIPR_XPSTTS[$4], $5	; XPSTTS
	ld.h		VIPR_XPSTTS[$4], $6	; XPSTTS
	cmp		$6, $5			; test if different
	be		-6				; load $6 again

	add		-1, $3			; Decrement our counter
	be		12				; down to zero
	.jump		XPSTTS_Loop1

	jmp 		[$31]				; return

;----------------------------------------------------------------
; Functions: DelayNOP
;----------------------------------------------------------------
;  Delays based on Loop with NOP's
; Parameters:
; Dependency Regs:
;   $3     - Delay count
;   $4,$5,$6  - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. DelayNOP

lb. NOP_Loop1					; Loop n times
	nop
	nop
	nop
	nop
	nop
	add		-1, $3			; Decrement our counter
	be		12				; down to zero
	.jump		NOP_Loop1
	nop

	jmp 		[$31]				; return


;----------------------------------------------------------------
; Functions: addObj
;----------------------------------------------------------------
;  Appends a new obj to the end of the obj table 
; Parameters:
; Dependency Regs:
;   $3  - CharParam - pointer to list of char params
;   $4  - JX - Upper Left Coordinates of the whole thing
;   $5  - JY
;   $6  - JLon,JRon,JP - parallax and what screen to display on
;   $7  - Char Offset
;   $8,$9,$10,$11,$12,$13  - Scrach Reg
;
;   CharParam has the form
;    dh. count - Totl number of Chars
;    dh. Char# - what chare to display
;    dh. X     - position relative to upper left coordinates above
;    dh. Y     - position relative to upper left coordinates above
;     :   :
;    dh. Char#
;    dh. X 
;    dh. Y  
;  Returns
;     $7  - CharPtr - a pointer to the end of the char's in char mem
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;
; Note assumes first OBJ is blank
;----------------------------------------------------------------
lb. addObj
	;Push Schrach Registers on stack
	addi		-24,$29,$29
	st.w		$8,0[$29]
	st.w		$9,4[$29]
	st.w		$10,8[$29]
	st.w		$11,12[$29]
	st.w		$12,16[$29]
	st.w		$13,20[$29]

	movhi		6, $0, $9			;0x0005F800 => $9
	movea		0xf800, $9, $9		;pointer to VIP Register

	mov		$3, $12			;copy pointer to scrach reg
	ld.h		0[$12], $10			;count of the # of char's
	andi		0xFFFF, $10, $10		;zero sign bit
	add		0x02, $12			;increment pointer by 2

      ;Obj Mem Base
	movhi		4, $0, $13			;0x00040000 => $13
	movea		0xE000, $13, $13		;0x0003E000 => $13

lb. addObjLoop1
	;Pree-Increment the obj pointer, does not work (why?)
	;ld.h		VIPR_SPT3[$9], $8		;SPT3
	;shl		0x18, $8			;clear sign bit?
	;shr		0x18, $8
	;add		1, $8
	;st.h		$8, VIPR_SPT3[$9]		;SPT3

	;Pree-Increment the obj pointer
	add		1, $7
	mov		$7, $8			;copy obj pointer
	shl		3, $8				;Multiply by 8
	add		$13, $8			;pointer to our obj

	; initialize the obj
	ld.h		0[$12], $11			;Char#
	st.h		$11, 6[$8]			;JCA
	ld.h		2[$12], $11			;relative X
	add		$4, $11			;add to base
	st.h		$11, 0[$8]			;JX
	ld.h		4[$12], $11			;relative Y
	add		$5, $11			;add to base
	st.h		$11, 4[$8]			;JY
	st.h		$6, 2[$8]			;JP
	add		0x06, $12			;increment pointer by 6
	
	;Decrement counter
	add		-1, $10			;decrement counter
	bn		12				; if zero end
	.jump		addObjLoop1			;goto loop top

	; Write obj pointer
	st.h		$7, VIPR_SPT3[$9]		;SPT3


	;Pop Schrach Registers from stack
	ld.w		 0[$29], $8
	ld.w		 4[$29], $9
	ld.w		 8[$29], $10
	ld.w		12[$29], $11
	ld.w		16[$29], $12
	ld.w		20[$29], $13
	addi		24, $29, $29

	jmp 		[$31]				; return

;----------------------------------------------------------------
; Functions: moveObj
;----------------------------------------------------------------
;-- Move Relative -- move the whole structure at once, relative to its last 
;position, convert to absolute position and call 'move'
;-- Move -- move the whole structure at once
;  Moves a given OBJ in the obj table 
; Parameters:
; Dependency Regs:
;   $3  - CharParam - pointer to list of char params
;   $4  - JX - Upper Left Coordinates of the whole thing
;   $5  - JY
;   $6  - JLon,JRon,JP - parallax and what screen to display on
;   $7  - Char Offset - base offset in char table
;   $8,$9,$10,$11,$12,$13  - Scrach Reg
;
;   CharParam has the form
;    dh. count - Totl number of Chars
;    dh. Char# - what chare to display
;    dh. X     - position relative to upper left coordinates above
;    dh. Y     - position relative to upper left coordinates above
;     :   :
;    dh. Char#
;    dh. X 
;    dh. Y  
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;
; Note assumes first OBJ is blank
;----------------------------------------------------------------

lb. moveObj
	;Push Schrach Registers on stack
	addi		-24,$29,$29
	st.w		$8,0[$29]
	st.w		$9,4[$29]
	st.w		$10,8[$29]
	st.w		$11,12[$29]
	st.w		$12,16[$29]
	st.w		$13,20[$29]

	mov		$3, $12			;copy pointer to scrach reg
	ld.h		0[$12], $10			;count of the # of char's
	andi		0xFFFF, $10, $10		;zero sign bit
	add		0x02, $12			;increment pointer by 2

      ;Obj Mem Base
	movhi		4, $0, $13			;0x00040000 => $13
	movea		0xE000, $13, $13		;0x0003E000 => $13

lb. moveObjLoop1
	;Pree-Increment the obj pointer
	add		1, $7
	mov		$7, $8			;copy obj pointer
	shl		3, $8				;Multiply by 8
	add		$13, $8			;pointer to our obj

	; initialize the obj
	ld.h		2[$12], $11			;relative X
	add		$4, $11			;add to base
	st.h		$11, 0[$8]			;JX
	ld.h		4[$12], $11			;relative Y
	add		$5, $11			;add to base
	st.h		$11, 4[$8]			;JY
	st.h		$6, 2[$8]			;JP
	add		0x06, $12			;increment pointer by 6
	
	;Decrement counter
	add		-1, $10			;decrement counter
	bn		12				; if zero end
	.jump		addObjLoop1			;goto loop top

	;Pop Schrach Registers from stack
	ld.w		 0[$29], $8
	ld.w		 4[$29], $9
	ld.w		 8[$29], $10
	ld.w		12[$29], $11
	ld.w		16[$29], $12
	ld.w		20[$29], $13
	addi		24, $29, $29

	jmp 		[$31]				; return

;----------------------------------------------------------------
; Functions: movBGMap
;----------------------------------------------------------------
; Takes a BGMap that points to char table 0, and repoints it to char table 1-3
; Parameters:
;   $3     - # of BGMap to relocate
;   $4     - # of Char Table to point to
; Dependency Regs:
;   $5, $6, $7     - Scrach Reg
;
; $1,2     - Compiler Scratch Reg's
; $29	     - Stack pointer, TopDown, PreeDecrement
; $31      - Call PC value (use jal or .call).
; $0       - Always 0 value
;----------------------------------------------------------------
lb. movBGMap
	andi		0x000D, $3, $3		;bgmap 0-13	only
	andi		0x0003, $4, $4		;char 0-3 only
	movea		0x2000, $0, $5		;Offset of one BGMap
	mul		$3, $5			;Base pointer
	movhi		0x0002, $0, $6		;BGMap Base Pointer
	add		$5, $6			;true pointer to BGMap
	movea		0x0200, $0, $7
	mul		$4, $7			;Offset to add to char.
	movea		0x2000, $0, $5		;Counter
lb. movBGMapLoop
	ld.h		0[$6],$8			; copy our data
	andi		0x007FF, $8, $9		;single out char pointer
	add		$7, $9			;increment count
	andi		0x007FF, $9, $9
	andi		0xF800, $8, $8		;Clear old value
	or		$9, $8			;put back together
	st.h		$8, 0[$6]			;update entry

	add		2, $6				; increment our Source/Dest pointers
	add		-2, $5			; Decrement our counter
	be		12				; if Zero, exit
	.jump		movBGMapLoop		; else loop to start
	jmp 		[$31]				; return
