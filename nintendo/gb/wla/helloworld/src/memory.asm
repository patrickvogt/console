;*
;* MEMORY.ASM - Memory Manipulation Code
;* by GABY. Inspired by Carsten Sorensen & others.
;*
;* V1.0 - Original release
;*


; Macro that pauses until VRAM available.

.MACRO lcd_WaitVRAM
-        ld      a,[rSTAT]       ; <---+
        and     STATF_BUSY      ;     |
        jr      nz,-          ; ----+
.ENDM

 ;       PUSHS           ; Push the current section onto assember stack.

.ORG $61

;***************************************************************************
;*
;* mem_Set - "Set" a memory region
;*
;* input:
;*    a - value
;*   hl - pMem
;*   bc - bytecount
;*
;***************************************************************************
mem_Set:
	inc	b
	inc	c
	jr	skip1
loop1:	ld	[hl+],a
skip1:	dec	c
	jr	nz,loop1
	dec	b
	jr	nz,loop1
	ret

;***************************************************************************
;*
;* mem_Copy - "Copy" a memory region
;*
;* input:
;*   hl - pSource
;*   de - pDest
;*   bc - bytecount
;*
;***************************************************************************
mem_Copy:
	inc	b
	inc	c
	jr	skip2
loop2:	ld	a,[hl+]
	ld	[de],a
	inc	de
skip2:	dec	c
	jr	nz,loop2
	dec	b
	jr	nz,loop2
	ret

;***************************************************************************
;*
;* mem_Copy - "Copy" a monochrome font from ROM to RAM
;*
;* input:
;*   hl - pSource
;*   de - pDest
;*   bc - bytecount of Source
;*
;***************************************************************************
mem_CopyMono:
	inc	b
	inc	c
	jr	skip3
loop3:	ld	a,[hl+]
	ld	[de],a
	inc	de
        ld      [de],a
        inc     de
skip3:	dec	c
	jr	nz,loop3
	dec	b
	jr	nz,loop3
	ret


;***************************************************************************
;*
;* mem_SetVRAM - "Set" a memory region in VRAM
;*
;* input:
;*    a - value
;*   hl - pMem
;*   bc - bytecount
;*
;***************************************************************************
mem_SetVRAM:
	inc	b
	inc	c
	jr	skip4
loop4:   push    af
        di
        lcd_WaitVRAM
        pop     af
        ld      [hl+],a
        ei
skip4:	dec	c
	jr	nz,loop4
	dec	b
	jr	nz,loop4
	ret

;***************************************************************************
;*
;* mem_CopyVRAM - "Copy" a memory region to or from VRAM
;*
;* input:
;*   hl - pSource
;*   de - pDest
;*   bc - bytecount
;*
;***************************************************************************
mem_CopyVRAM:
	inc	b
	inc	c
	jr	skip5
loop5:   di
        lcd_WaitVRAM
        ld      a,[hl+]
	ld	[de],a
        ei
	inc	de
skip5:	dec	c
	jr	nz,loop5
	dec	b
	jr	nz,loop5
	ret

;        POPS           ; Pop the current section off of assember stack.

