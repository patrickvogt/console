.define FlagMask %11111111		; Mask for flag register
;                 SZXHXPNC
;                 |||||||`- Carry
;                 ||||||`-- Add/subtract
;                 |||||`--- Parity/overflow
;                 ||||`---- Undocumented
;                 |||`----- Half carry
;                 ||`------ Undocumented
;                 |`------- Zero
;                 `-------- Sign

;==============================================================
; WLA-DX banking setup
;==============================================================
.MemoryMap
DEFAULTSLOT 0
SLOTSIZE $4000
SLOT 0 $0000  ; ROM
SLOT 1 $4000
SLOT 2 $8000  ; ROM (not used)
SLOT 3 $c000  ; RAM
.ENDME

.RomBankMap
BANKSTOTAL 2
BANKSIZE $4000
BANKS 2
.ENDRO

.bank 0 slot 0

.org $0000
.section "Boot section" force
  di
  im 1
  ld sp,$dff0
  jp Start
.ends

.org $0066
.section "Pause handler" force
push af
  ld a,(PauseFlag)
  xor 1             ; toggle flag
  ld (PauseFlag),a
-:ld a,(PauseFlag)
  cp 0            ; Loop if non-zero
  jr nz,-
pop af
retn
.ends

/*
.ramsection "Variables" slot 1
  CntBit   db      ; Counter and Shifter variables...
  CntByt   dw
  ShfBit   db
  ShfByt   dw
  Counter  dsw 20
  Shifter  dsw 20
  MachineStateBeforeTest     dsb 14  ; Machine state before test
  StackPointerBeforeTest     dw      ; sp before test
  MachineStateAfterTest     dsb 14  ; Machine state after test
  StackPointerAfterTest     dw      ; sp after test
  StackPointerSaved    dw      ; Saved sp
  CRCValue   dsb 4   ; CRC value
.ends
.define MachineStateBeforeTestHi  MachineStateBeforeTest >> 8 ;/ $100
.define MachineStateBeforeTestLo  MachineStateBeforeTest & $ff
*/

.define CntBit  $C000
.define CntByt  $C001
.define ShfBit  $C003
.define ShfByt  $C004
.define Counter $C010
.define Shifter $C038
.define MachineStateBeforeTest    $C070
.define StackPointerBeforeTest    $C07E
.define MachineStateBeforeTestHi  MachineStateBeforeTest / $100
.define MachineStateBeforeTestLo  MachineStateBeforeTest & $ff
.define MachineStateAfterTest    $C080  ; Machine state after test
.define StackPointerAfterTest    $C08E  ; Stack pointer after test
.define StackPointerSaved   $C090  ; Saved stack pointer
.define CRCValue  $C092  ; CRC value

; SMS drawing routines variables
.define CursorX $C300
.define VRAMAdr $C304
.define Scroll  $C306
.define ScrollF $C307
.define PrintSl $C390
.define Test    $C400

.define PauseFlag $d000

; For the purposes of this test program, the machine state consists of:
; a 2 byte memory operand, followed by
; the registers iy,ix,hl,de,bc,af,sp
; for a total of 16 bytes.

; The program tests instructions (or groups of similar instructions)
; by cycling through a sequence of machine states, executing the test
; instruction for each one and running a 32-bit crc over the resulting
; machine states.  At the end of the sequence the crc is compared to
; an expected value that was found empirically on a real Z80.

; A test case is defined by a descriptor which consists of:
; a flag mask byte,
; the base case,
; the increment vector,
; the shift vector,
; the expected crc,
; a short descriptive message.
;
; The flag mask byte is used to prevent undefined flag bits from
; influencing the results.  Documented flags are as per Mostek Z80
; Technical Manual.
;
; The next three parts of the descriptor are 20 byte vectors
; corresponding to a 4 byte instruction and a 16 byte machine state.
; The first part is the base case, which is the first test case of
; the sequence.  This base is then modified according to the next 2
; vectors.  Each 1 bit in the increment vector specifies a bit to be
; cycled in the form of a binary Counter.  For instance, if the byte
; corresponding to the accumulator is set to $ff in the increment
; vector, the test will be repeated for all 256 values of the
; accumulator.  Note that 1 bits don't have to be contiguous.  The
; number of test cases 'caused' by the increment vector is equal to
; 2^(number of 1 bits).  The shift vector is similar, but specifies a
; set of bits in the test case that are to be successively inverted.
; Thus the shift vector 'causes' a number of test cases equal to the
; number of 1 bits in it.

; The total number of test cases is the product of those caused by the
; Counter and shift vectors and can easily become unweildy.  Each
; individual test case can take a few milliseconds to execute, due to
; the overhead of test setup and crc calculation, so test design is a
; compromise between coverage and execution time.

; This program is designed to detect differences between
; implementations and is not ideal for diagnosing the causes of any
; discrepancies.  However, provided a reference implementation (or
; real system) is available, a failing test case can be isolated by
; hand using a binary search of the test space.

Start:
  ; Initialisation
  call SMSInitialise
  ld de,msg1
  ld c,9
  call OutputText

-:ld a,(hl)
  inc hl
  or (hl)
  jp z,+  ; finish if 0000
  dec hl
  jp -

-:jp -  ; Infinite loop to stop program

; MessageString for defining string constants
.macro MessageString
  .db \1,'$'
  ; No message length checking :P
.endm

OutputText:
  push af
  push bc
  push de
  push hl
; Accept call 2 (print char) and call 9 (print string)
; Ignore all others
    ld b,a  ; save char (destroys B)
    ld a,c
    cp 2
    jr z,PrintChar
    cp 9
    jr z,PrintString
OutputTextdone:
  pop hl
  pop de
  pop bc
  pop af
  ret

; Pass OutputText calls to display code
PrintChar:
  ld a,b  ; get char back
  cp 10  ; ignore LF, CR auto-LFs
  jr z,OutputTextdone
  call smsprint
  jr OutputTextdone

PrintString:
  ld a,(de)
  cp '$'
  jr z,OutputTextdone
  cp 10
  call nz,smsprint
  inc de
  jr PrintString

; Messages
msg1:   .db "Hello World",10,13,10,13,"$"
crlf:   .db 10,13,"$"

; SMS-specific stuff


; (erq) Moved install to top, since it is the same for both
; (erq) VDP and SDSC Debug Console versions of ZEXALL.

; Copies the Test code into modifiable memory.
; This is because the code is self-modifying.
Install:
  push bc
  push de
  push hl
    ld de,Test
    ldir
  pop hl
  pop de
  pop bc

  ret

font:
.incbin "../bin/pattern_table_1.chr"
fontend:
.include "vdp.inc"

SMSInitialise:
  push af
  push bc
  push de
  push hl
    ld a,0
    ld (PauseFlag),a
    call SetUpVDP
    call LoadFont
    call LoadPalette
    call InitCursor
    call Install ; first, install Test code into RAM

    ; Turn screen on
    ld a,%11000000
;         ||||| |`- Zoomed sprites -> 16x16 pixels
;         ||||| `-- Doubled sprites -> 2 tiles per sprite, 8x16
;         ||||`---- 30 row/240 line mode
;         |||`----- 28 row/224 line mode
;         ||`------ VBlank interrupts
;         |`------- Enable display
;         `-------- Must be set (VRAM size bit)
    out ($bf),a
    ld a,$81
    out ($bf),a

  pop hl
  pop de
  pop bc
  pop af
  ret

; Set up our VDP for display. Set the correct video mode, and enable display
SetUpVDP:
  ld hl,vdpregs
  ld b,18
  ld c,$bf
  otir
  ret

; Install our palette into the VDP
LoadPalette:
  ; Set VRAM address
  ld a,$00
  out ($bf),a
  ld a,$c0
  out ($bf),a
  ld hl,palette
  ld b,$20
  ld c,$be
  otir
  ret

; Load the font. Also clears the VRAM first.
LoadFont:
  ld hl,$4000    ; Clear VRAM
  ld bc,1
  ld a,0
-:out ($be),a
  sbc hl,bc
  jr nz,-

  ld a,$00       ; Set VRAM write address
  out ($bf),a
  ld a,$60
  out ($bf),a

  ld hl,font
  ld bc,fontend-font
  ld de,1
-:ld a,(hl)
  out ($be),a
  out ($be),a
  out ($be),a
  out ($be),a
  inc hl
  dec c
  jr nz,-
  djnz -
  ret

; Initialize the cursor variables
InitCursor:
  ld hl,0
  ld a,0
  ld (CursorX),a
  ld (VRAMAdr),hl
  ld (Scroll),a
  ld (ScrollF), a
  ret

; Code to wait for Start of blanking period
WaitForVBlank:
  push af
  push bc
 --:in a,($7e)  ; get VCount value
  -:ld b,a      ; store it
    in a,($7e)  ; and again
    cp b        ; Is it the same?
    jr nz,-     ; If not, repeat
    cp $c0      ; Is is $c0?
    jp nz,--    ; Repeat if not
  pop bc
  pop af
  ret


; Code to print a character on the screen. Does stuff like handle
; line feeds, scrolling, etc.

smsprint:
  push af
  push bc
  push de
  push hl
    call WaitForVBlank

    ; First, write the character (in a) to the screen
    ; If it's a carriage return, skip it.
    cp 13
    jp z,doneprint
    ; Otherwise, we write.
    ld b,a        ; Save the output value
    ld c,$bf      ; Control port
    ld hl,(VRAMAdr)
    ld a,$78      ; Set nametable base bits
    or h          ; Get remaining bits

    out (c),l     ; Output lower-order bits
    out (c),a     ; Output upper bits + control

    ld a,b        ; Put value back in A
    sub $20       ; Shift into our font range

    ld c,$be
    out (c),a     ; Output the character

    ld a,b        ; Put value back in A... again

    ld b,1
    out (c),b

    ; Move cursor forward
    push af
      ld a,(CursorX)
      inc a
      ld (CursorX),a
    pop af

    ; Update VRAM pointer
    inc hl
    inc hl
    ld (VRAMAdr),hl

    ; Now the fun computation
doneprint:
    ; Check if this was a carriage return.
    cp 13
    jp z,nextline
    ; Check if we're at the end of the line
    ld a,(CursorX)
    cp 32
    jp z,nextline
  pop hl
  pop de
  pop bc
  pop af
  ret

  ; Here we do the job of scrolling the display, computing the
  ; new VRAM address, and all that fun stuff.
nextline:
    ld hl,(VRAMAdr)     ; Increase the VRAM position
    ; Get the cursor position and find out how far it was to the
    ; end of the line.
    ld a,(CursorX)
    ld b,a
    ld a,32
    sub b
    sla a               ; Now, double this and add it to HL. This is the new address.
    ld c,a
    ld b,0
    call namefill       ; Fill the rest of the line
    add hl,bc           ; Now create new address.
    ccf                 ; Next, check if we're past the end of the screen.
    push hl
      ld bc,$05C0
      sbc hl,bc
      jp m,+
      ld a,1            ; If we are, set the scroll flag on... we scroll from now on.
      ld (ScrollF),a
  +:pop hl
    push hl
      ld bc,$0700       ; Next, check if we're at the end of VRAM
      sbc hl,bc
    pop hl
    jp m,+
    ld hl,0             ; If we are, return to the top of VRAM.
  +:ld (VRAMAdr),hl     ; Now, save our VRAM address.
    ld bc,$40           ; Clear the new line
    call namefill
    ld a,(ScrollF)      ; Load the Scroll flag and check if it's set.
    cp 0
    jp z,noScroll
    ld a,(Scroll)       ; If it is, increase the Scroll value, and wrap at 28.
    inc a
    cp 28
    jp nz,doScroll
    ld a,0
doScroll:
    ld (Scroll),a       ; Now, write out Scroll value out to the VDP.
    sla a
    sla a
    sla a
    ld c,$bf
    out (c),a
    ld a,$89
    out (c),a
noScroll:
    ld a,0              ; Reset the cursor X position to 0
    ld (CursorX),a
  pop hl
  pop de
  pop bc
  pop af
  ret

; Fill the nametable from HL with BC bytes.
namefill:
  push af
  push bc
  push de
  push hl
    ld a,b      ; Wait for blanking period
    or c
    jp z,+
    push bc     ; Save the Counter for later.
      ld c,$bf  ; Control port
      ld a,$78  ; Set nametable base bits
      or h      ; Get remaining bits
      out (c),l ; Output lower-order bits
      out (c),a ; Output upper bits + control
      ; Now, zero out the region
      pop hl    ; Get our Counter
      ld a,0
      ld de,1
      ld c,$be
      ccf
    -:out (c),a ; Output the character
      sbc hl,de
      jp nz,-
+:pop hl
  pop de
  pop bc
  pop af
  ret

;==============================================================
; SDSC tag and SMS rom header
;==============================================================
.sdsctag 0.0,"Hello World",SDSCNotes,"Patrick Vogt"

SDSCNotes:
.db 0
