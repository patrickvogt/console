.INCLUDE "nes.inc"

.GLOBALZP nmis

.P02

.SEGMENT "ZEROPAGE"

nmis: .RES 1 

.SEGMENT "INES_HEADER"

  .BYT "NES", $1A
  .BYT 1
  .BYT 1
  .BYT %00000001 
  .BYT %00000000

.SEGMENT "VECTORS"
  .ADDR nmi, reset, irq

.SEGMENT "CODE"

.PROC irq
  rti
.ENDPROC

.PROC nmi
  inc nmis
  rti
.ENDPROC

.PROC reset
  
  sei
  cld           

  ldx #0
  stx PPUMASK   
    
  ldx #$FF       
  txs     

  bit PPUSTATUS    

vwait1:

  bit PPUSTATUS
  bpl vwait1

  ldy #0
  ldx #$00 ;


clear_zp:
  sty $00,x
  inx   
  bne clear_zp
  
vwait2:
  bit PPUSTATUS
  bpl vwait2

start:
  jsr draw_helloworld

  lda #VBLANK_NMI
  sta PPUCTRL

  lda nmis
:
  cmp nmis
  beq :-

  lda #0 
  sta PPUSCROLL
  sta PPUSCROLL

  lda #BG_1000|OBJ_1000
  sta PPUCTRL

  lda #BG_ON|OBJ_ON
  sta PPUMASK

loop:
  jmp loop

.ENDPROC 

.PROC clear_screen
  lda #$20
  ldx #$00

  sta PPUADDR
  stx PPUADDR

  ldx #240

:
  sta PPUDATA 
  sta PPUDATA
  sta PPUDATA
  sta PPUDATA

  dex 
  bne :- 
  ldx #64 
  lda #0

:
  sta PPUDATA 
  dex
  bne :-

  rts 
.ENDPROC

.PROC draw_helloworld

  jsr clear_screen

  lda #$3F
  sta PPUADDR
  lda #$00
  sta PPUADDR

  ldx #8
:
  
  lda #$0F
  sta PPUDATA

  sta PPUDATA
  sta PPUDATA

  lda #$30
  sta PPUDATA
  
  dex
  bne :-

  lda #>helloWorldText
  sta $01

  lda #<helloWorldText
  sta $00

  lda #$20
  sta $03
  lda #$82
  sta $02

  jsr print_hello_world
 
  rts
.ENDPROC

.PROC print_hello_world 
dstLo = $02
dstHi = $03 
src = $00 

  lda dstHi
  sta PPUADDR
  lda dstLo
  sta PPUADDR

  ldy #0
loop:
  lda (src),y
  beq done
  
  iny
  bne :+

  inc src+1

: 
  cmp #10
  beq newline

  sta PPUDATA 

  bne loop

newline: 

  lda #32 

  clc 

  adc dstLo
  sta dstLo 

  lda #0 

  adc dstHi
  sta dstHi 
  sta PPUADDR
  lda dstLo
  sta PPUADDR
  
  jmp loop 
done:
  rts 
.ENDPROC

.SEGMENT "DATA"

helloWorldText:
  .BYT "Hello",$20,"World",0
