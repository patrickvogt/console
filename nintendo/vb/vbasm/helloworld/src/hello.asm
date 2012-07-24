;----------------------------------------------------------------
; Reality Boy, demo 1
;  a simple tester to develop new librarys for 
;  programming the VB, as well as some practice in asm.
;----------------------------------------------------------------
; Original coded by:
;       (C)1999, 2000 David Tucker
;       dbt@dana.ucc.nau.edu
;       http://dana.ucc.nau.edu/~dbt/VBMain.html
;----------------------------------------------------------------
; ToDo:
;   BGMap demo - scrolling, multipel layers, transparency
;   Special Effects demo - pallet, Affin, H-Bias, Column Table
;   OBJ demo - Multiple Objs, Animation (walking)
;   Add a menu
;
;----------------------------------------------------------------

	org.		0x07000000			; Start of rom

;----------------------------------------------------------------
; Program Constants (Char Ram, etc)
;----------------------------------------------------------------
	inc.		"vb_defines.inc"		; Defenition of memory space
	inc.		"vb_func.inc"		; Standard Function's

	inc.		"focus.inc"			; Focus Screen
	inc.		"control.inc"		; Controller Screen Fn's

	lb.		ProgStart			; The start of the real program.

;----------------------------------------------------------------
;  VB init
;----------------------------------------------------------------
	.call		VBReset			; Initialize the VB

;----------------------------------------------------------------
;  Focus Screen
;----------------------------------------------------------------
	.call		LoadFocusImg		; Load the Focus Screen
	.call		DspOn				; Turn on the Display 
	.call		DspPalOn			; Turn on the Pallet 

;----------------------------------------------------------------
;  Read Controller
;----------------------------------------------------------------
	.call		ReadPad			; Clear the controller buffer

	; check if key pressed
lb.	rdCtrl001
	.call		ReadPad			; Read the controller
	andi		0xFFFC, $3, $3		; Any button pressed
	bne		12				; if not Zero, exit
	.jump		rdCtrl001			; else loop to start

	; turn off Display
	.call		DspPalOff			; turn off the Pallet
	.call		DspOff			; Turn off the Display 

	; Make shure key is released
lb.	rdCtrl002
	.call		ReadPad			; Read the controller
	andi		0xFFFC, $3, $3		; Any button pressed 0xFFFC
	be		12				; if Zero, exit
	.jump		rdCtrl002			; else loop to start

;----------------------------------------------------------------
;  Controller Screen
;----------------------------------------------------------------
	.call		LoadCtrlImg			; Load our Controller image
	.call		DspOn				; Turn on the Display 
	.call		DspPalOn			; Turn on the Pallet 

;Place the OBJ's in the screen
lb.	ctrlScrLoop001

	.call		RunCtrlImg			;Display button presses

	;-- Delay for 0x2000 cycles --
	mov		$0, $3			;0x00000000 => $3
	movea		0x2000, $0, $5		;0x00002000 => $5
	jr		6				;Jump to LoopStart
	;; loop top
	add		1, $3				;increment counter
	;; loop start
	cmp		$5, $3			;end of loop?
	blt		-4				;if not goto LoopTop

;	;-- Wait for retrace, does not work? --
;	;; Pointer to VIP mem
;	movhi		6, $0, $3			;0x0005F800 => $3
;	movea		0xf800, $3, $3		;pointer to VIP Register
;	;; loop top
;	ld.h		VIPR_DPSTTS[$3], $4	;load DPSTTS in $4
;	andi		0xffff, $4, $4		;look at lower 16 bytes
;	;andi		0x40, $4, $4		;is bit 6 on?
;	andi		0x3C, $4, $4		;mask with 0x3C
;	be		-12				; if not goto loop top

	.jump		ctrlScrLoop001		; Loop Indefinently

;----------------------------------------------------------------
; Rom Info...         
;----------------------------------------------------------------

	org.	0x0707FDE0			; Game Title, 20 chrs...
	ds. "DEMO1 By David T.   "
        
	org.	0x0707FDF4			; Reserved
	db.	00, 00, 00, 00, 00
        
	org.	0x0707FDF9			; Manufac Code
	ds. "DB"				; David Bruce, this is my Code =0)
						; Change it to whatever you want.
	org.	0x0707FDFB			; Game Code
	ds. "DEM1"
        
	org.	0x0707FDFF			; Rom Ver
	db.	00

;----------------------------------------------------------------
; Interrupt Handlers...
;----------------------------------------------------------------
	org.	0x0707FE00			; Key Interupt
	reti
	org.	0x0707FE10			; Timer Interupt
	reti
	org.	0x0707FE20			; Expantion Port Interupt?
	reti
	org.	0x0707FE30			; Link Port Interupt
	reti
	org.	0x0707FE40			; Video Refresh Interupt
	reti

;----------------------------------------------------------------
; Exception Handlers, probably not necisary...
;----------------------------------------------------------------
	org.	0x0707FF60			;Floating Point Exception (various)
	reti
	org.	0x0707FF70			;???
	reti
	org.	0x0707FF80			;Divide By Zero
	reti
	org.	0x0707FF90			;Invalid Opcode
	reti
	org.	0x0707FFA0			;Trap Instruction (param 0x0n)
	reti
	org.	0x0707FFB0			;Trap Instruction (param 0x1n)
	reti
	org.	0x0707FFC0			;Address Trap
	reti
	org.	0x0707FFD0			;Non Maskable Interupt/Douplexed Exception
	reti
	org.	0x0707FFE0			;Fatal Exception
	reti
        
;----------------------------------------------------------------
; Reset Vector... 
;----------------------------------------------------------------
	org.	0x0707FFF0			; Reset Vector
	.jump	ProgStart			; Trying jump to start
	nop                           
	nop                           
	nop

