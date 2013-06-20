; WLA ROM stuff

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

.sdsctag 1.0,"USB Demo","USB Test","pvogt"
.bank 0 slot 0

; some useful names

.define KEY_REG         $fff9
.define FIFO_PORT        $7f02
.define IS_FIFO_WR_BUSY %00100000
.define STATE_PORT      $7f01

; boot code

.org $0000

	    di
	    im 1

	    jp main

        ; try USB
    
        ;unlock registers
main:   ld hl,KEY_REG
        ;KEY_REG = 0;
        ld a,0
        ld (hl),a
    
        ;KEY_REG = 0x52;
        ld a,$52
        ld (hl),a
    
        ;KEY_REG = 0x45;
        ld a,$45
        ld (hl),a
    
        ;KEY_REG = 0x4e;
        ld a,$45
        ld (hl),a
    
        ;FIFO_WR_BUSY?
fifo:   ld hl,STATE_PORT
        ld a,(hl)
        ld b,IS_FIFO_WR_BUSY
        and b
        jp nz,fifo
     
        ;WRITE
     
        ld hl,FIFO_PORT
    
        ;inf loop FIFO_PORT = 0;
write:  ld a,0
        ld (hl),a
        jp write