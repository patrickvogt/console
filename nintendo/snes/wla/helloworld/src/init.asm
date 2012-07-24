; Alle Quelldateien befinden sich im src/-Ordner
.INCDIR "./src"

; 'snes.inc' beinhaltet ein paar symbolische Namen für die einzelnen Register
;  des SNES und smbolische Namen für häufig verwendete Bitmasken
.INCLUDE "snes.inc"

;-------------------------------------------------------------------------------
; init_snes -- Initialisierung von SNES-Speichers und einzelnen Registern
;-------------------------------------------------------------------------------
.MACRO init_snes
  ; Schalte alle Interrupts ab (bis auf NMI)
  sei
  
  ; Wir wollen auf jeden Fall im 'native mode' arbeiten
  clc                     
  xce 

  ; -Akkumulator auf 16 Bit setzen
  ; -X-/Y-Register auf 16 Bit setzen
  ; -Dezimal-Modus abschalten (wir arbeiten binär)
  rep #P_N_ACCU_REGISTER_SIZE|P_N_INDEX_REGISTER_SIZE|P_DECIMAL		
			
  ; Stapelzeiger initialisieren
  ldx #$1FFF	
  txs

  ; Zum Unterprogramm 'initialize_snes' springen
  jsl initialize_snes.L

  ; Akkumulator auf 8 Bit-Modus setzen
  sep #P_N_ACCU_REGISTER_SIZE		
.ENDM

;-------------------------------------------------------------------------------

.BANK 0 SLOT 0
.ORG 0
.SECTION "InitializeSNESCode" FORCE

initialize_snes:
  ; Data Bank = Program Bank
  phk	 
  plb

  ; Setze Direct Page = $0000 und transferiere den Inhalt in das Direct-Register
  lda #$0000 
  tcd		

  ; Bevor wir den Speicher löschen, müssen wir noch die aktuelle 
  ;  Rücksprungadresse (24 Bit) vom Stack sichern, da dieser gleich 
  ;  überschrieben wird
  ldx $1FFD		
  stx $4372  	
  ldx $1FFF
  stx $4374

  ; Akkumulator in 8 Bit-Modus und X-/Y-Register in 16 Bit-Modus schalten
  sep #P_N_ACCU_REGISTER_SIZE
  rep #P_N_INDEX_REGISTER_SIZE

  ; Bildschirm ausschalten
  lda #SCREEN_OFF
  sta SCREEN_DISPLAY

  ; Zählvariable, um die Register $2101-$210C zu 'nullen'
  ldx #$2101

; Die folgende Schleife 'nullt' die Register $2101-$210C zu 'nullen'.
;  Dieser Adressbereich beinhaltet Daten / Eigenschaften der einzelnen 
;  Hintergründe
_loop_00:		
  ; 'nulle' das aktuelle Register mithilfe der indizierten Adressierung
  stz $00,x
  ; Inkrementiere den Inhalt des X-Registers
  inx
  ; Haben wir zuletzt $210C 'genullt'?
  cpx #$210D
  ; Wenn nicht, dann springen wir wieder nach oben (zu '_loop_00')
  bne _loop_00

; Jetzt müssen die Register $210D-$2114 geleert werden
;  Diese Register müssen separat behandelt werden, da wir diese Register 2 mal
;  nullen müssen (jeweils Low- und High-Byte der Register)
;
; Diese Register beinhalten Werte für das Scrolling. Für uns momentan unwichtig,
;  da wir noch nicht 'scrollen' werden
_loop_01:	
  ; Nulle Low- und High-Byte der Register	
  stz $00,x		
  stz $00,x
  ; Inkrementiere den Inhalt des X-Registers bzw. des Adressoffsets
  inx
  ; Haben wir zuletzt $2114 'genullt'?
  cpx #$2115
  ; Wenn nicht, dann springen wir wieder nach oben (zu '_loop_01')
  bne _loop_01

  ; Definiere den VRAM 'transfer mode' als 'word access'
  lda #%10000000
  sta VIDEO_CONTROL

  ; Setze die VRAM port-Adresse auf $0000
  stz VRAM_ADDRESS_L		
  stz VRAM_ADDRESS_H		

	; Register $2118-$2119 müssen nicht initialisiert werden

  ; Lösche eventuelle Einstellungen vom 'Mode 7'
  stz $211A

  ; Bereite die Zählvariable / den Adressoffset vor, um den nächsten Bereich 
  ;  ($211B-$2120) zu nullen
  ldx #$211B
_loop_02:		
  ; Diese Register, die die Matrizen des Mode 7 definieren müssen auch 2-mal 
  ;  genullt werden 
  stz $00,x
  stz $00,x
  ; Inkrementiere den Inhalt des X-Registers bzw. unseren Adressoffset
  inx
  
  ; Wenn wir noch nicht fertig sind, springe nach oben (zu '_loop_02')
  cpx #$2121
  bne _loop_02

	; Register $2121-$2122 müssen nicht initialisiert werden

  ; Bereite die Zählvariable / den Adressoffset vor, um den nächsten Bereich 
  ;  ($2123-$2133) zu nullen
  ldx #$2123
_loop_03:
  ; Nulle das aktuelle Register
  stz $00,x		
  inx			
    
  ; Wenn wir noch nicht fertig sind, springe nach oben (zu '_loop_03')
  cpx #$2134	
  bne _loop_03

	; Register $2134-$213D müssen nicht initialisiert werden

  ; Teilweise ist die Funktionsweise einzelner Bits dieses Register unbekannt.
  ;  Wir nullen einfach mal das gesamte Register (??)
  stz $213E
			
  ; Register $213F-$2143 müssen nicht initialisiert werden

  ; Register $2180-$2183 müssen nicht initialisiert werden

	; Register $4016-$4017 müssen nicht initialisiert werden

  ; NMI ausschalten, IRQ ausschalten
  stz $4200

  ; (??)
  lda #$FF
  sta $4201

  ; Register $4202-$420A müssen nicht initialisiert werden

  ; 'normale' DMA-Kanäle deaktivieren
  stz $420B
  ; HDMA-Kanäle deaktivieren 
  stz $420C

  ; Wir erzeugen ein SlowROM
  stz $420D

  ; NMI-Status lesen, dies löscht automatisch das NMI-Flag, falls es gesetzt
  ;  sein sollte
  lda $4210

	; Register $4211-421f müssen nicht initialisiert werden

  jsr clear_vram      ;Reset VRAM
  jsr clear_palette   ;Reset colors

  ; lösche 'Sprite tables' (??)

  ldx #$0080
  lda #$F0
_loop_04:
  sta $2104	
  sta $2104	
  stz $2104	
  stz $2104
  dex
  bne _loop_04

  ldx #$0020
_loop_05:
  stz $2104
  dex
  bne _loop_05

  ; WRAM löschen via DMA (??)

  stz $2181		
  stz $2182
  stz $2183

  ; $08 wird nach DMA_CH0_CONTROL geschrieben und 
  ;  $80 implizit nach DMA_CH0_DESTINATION. Damit ist $2180 die Zieladresse des
  ;  folgenden DMA-Transfers
  ldx #$8008
  stx DMA_CH0_CONTROL        
  ldx #wram_fill_byte
  stx DMA_CH0_SOURCE_OFFSET         
  lda #:wram_fill_byte
  sta DMA_CH0_SOURCE_BANK  
  ; 0 Bytes (??)     
  ldx #$0000
  stx DMA_CH0_NUMBER_OF_BYTES         
  lda #DMA_CH0_ENABLE
  sta DMA_CONTROL        

  lda #DMA_CH0_ENABLE        
  sta DMA_CONTROL  

  ; Data Bank = Program Bank
  phk 
  plb

  ; Interrupts wieder einschalten bzw. nicht maskieren
  cli

  ; Wir sind fertig, also können wir unsere Rücksprungadresse (24 Bit) wieder
  ;  zurück in den Stack schreiben
  ldx $4372
  stx $1FFD
  lda $4374
  sta $1FFF
  
  ; return from subroutine long
  rtl

; Mit diesem Byte wollen wir den WRAM befüllen / initialisieren
wram_fill_byte:
  .DB $00

;-------------------------------------------------------------------------------
; 'clear_vram' -- Initialisiert den gesamten VRAM mit $00
; Eingabeparameter: /
; Ausgabeparameter: /
; modifiziert: P-Register
;-------------------------------------------------------------------------------
clear_vram:
  ; 'callee save'
  ;  speichere (= push) den aktuellen Zustand des Akkumulators, des P-Registers 
  ;  und des X-Registers auf den Stack
  pha
  phx
  php

  ; Setze die Registerbreite des X-/Y- Registers auf 16 Bit
  rep #P_N_INDEX_REGISTER_SIZE
  ; Setze die Registerbreite vom Akkumulator auf 8 Bit
  sep #P_N_ACCU_REGISTER_SIZE

  ; 'VRAM port' auf 'word access' stellen,
  ;   Erst nach dem Schreibvorgang nach Adresse $2119 wird die Adresse im VRAM
  ;   inkrementiert (??)
  lda #%10000000
  sta VIDEO_CONTROL

  ; Hier werden mehrere Attribute für den folgenden DMA-Transfer definiert,
  ;
  ; Es handelt sich um einen ...
  ;  -... Transfer von Daten vom CPU-Speicher zu den PPU-Registern
  ;  -... es werden jeweils 2 Bytes nach $Adresse und $Adresse+1 geschrieben
  lda #%00001001
  sta DMA_CH0_CONTROL

  ; Hier wird die Zieladresse des DMA-Transfers festgelegt.
  ;
  ; Hinweis: Die Zieladresse besitzt automatisch das Prefix $21.
  ;  Über den folgenden Befehl wird bspw. als Zieladresse $2118 festgelegt.
  ;  Implizit wird auch $2119 als Adresse festgelegt, da wir weiter oben 
  ;  'word access' festgelegt haben
  lda #(VRAM_DATA & $00FF) 
  sta DMA_CH0_DESTINATION
  
  ; Setze die VRAM adresse auf $0000
  ;
  ; Hinweis: Da das X-Register im 16 Bit-Modus ist, wird hier automatisch in
  ;  $2116 und $2117 geschrieben. Das Low-Byte (Bit 7-Bit 0 (LSB)) wird in $2116
  ;  geschrieben. Das High-Byte (Bit 15 (MSB)-Bit 8) wird in $2117 geschrieben
  ldx #$0000
  stx VRAM_ADDRESS

  ; Setze die Quelladresse des DMA-Transfers auf $00:$0000
  stx $0000         
  stx DMA_CH0_SOURCE_OFFSET         
  lda #$00
  sta DMA_CH0_SOURCE_BANK        

  ; Setze die Anzahl der Bytes, die beim DMA-Transfer kopiert werden sollen
  ;  In diesem Fall sind dies 64K-1 Bytes
  ldx #$FFFF
  stx DMA_CH0_NUMBER_OF_BYTES

  ; Aktiviere nun den DMA-Transfer
  lda #DMA_CH0_ENABLE
  sta DMA_CONTROL      

  ; 'nulle' noch das letzte Byte des VRAMs
  stz VRAM_DATA_H

  ; 'reserve callee save'
  ;  hole (= pull) die zuvor gespeicherten Inhalte von P-Register und X-Register
  ;  vom Stack und transferiere diese in das P- bzw. X-Register
  plp
  plx
  pla

  ; return from subroutine - kehre zum Aufrufer zurück
  rts

;-------------------------------------------------------------------------------
; 'clear_palette' -- Initialisiert die gesamte Farbpalette mit $00 (Schwarz)
; Eingabeparameter: /
; Ausgabeparameter: /
; modifiziert: P-Register
;-------------------------------------------------------------------------------
clear_palette:
  ; 'callee save'
  ;  speichere (= push) den aktuellen Zustand des P-Registers und des 
  ;  X-Registers auf den Stack
  phx
  php

  ; Setze die Registerbreite des X-/Y- Registers auf 16 Bit
  rep #P_N_INDEX_REGISTER_SIZE
  ; Setze die Registerbreite vom Akkumulator auf 8 Bit
  sep #P_N_ACCU_REGISTER_SIZE

  ; Setze die Adresse für zukünftige CGRAM-Write Operationen auf $00
  stz CGRAM_ADDRESS
  ; Wir müssen insgesamt 256 Farbdefinitionen (jeweils 2 Byte) löschen / 
  ;  'nullen'. Dies ist also unsere interne Zählvariable
  ldx #256

clear_palette_loop:
  ; 'nulle' das Low-Byte (Bit 7-0 (LSB)) der 2-Byte breiten Farbdefinition
  stz CGRAM_DATA
  ; 'nulle' das High-Byte (Bit 15 (MSB)-8) der 2-Byte breiten Farbdefinition
  stz CGRAM_DATA 
  ; dekrementiere den Inhalt der X-Registers bzw. unsere Zählvariable
  dex
  ; Wenn wir noch nicht alle 256 Farben 'genullt' haben, dann springe nach oben
  bne clear_palette_loop

  ; 'reserve callee save'
  ;  hole (= pull) die zuvor gespeicherten Inhalte von P-Register und X-Register
  ;  vom Stack und transferiere diese in das P- bzw. X-Register
  plp
  plx

  ; return from subroutine - kehre zum Aufrufer zurück
  rts

.ENDS
