
; Die Header-Datei einbinden
;  Diese Datei definiert die Speicherarchitektur des SNES und die Metadaten des
;  SNES-Roms, den sogenannten 'SNES-Header'.
.INCLUDE "header.inc"

; Initialisierungscode eibinden. Diese Datei beinhaltet ein Makro, über die der
;  SNES initialisiert werden kann. (bspw. initialisieren / 'nullen' des 
;  Speichers etc.)
.INCLUDE "init.asm"

; 'snes.inc' beinhaltet ein paar symbolische Namen für die einzelnen Register
;  des SNES und smbolische Namen für häufig verwendete Bitmasken
.INCLUDE "snes.inc"

; 'load_graphics' beinhaltet Routinen zum Laden / Kopieren der Farbpalette und
;  für die einzelnen Kacheln (engl. tiles), die die druckbaren Zeichen 
;  definieren (ähnlich einer ASCII-Tabelle) 
.INCLUDE "load_graphics.asm"

; hier beginnt unser Hauptcode
.BANK 0 SLOT 0
.ORG 0
.SECTION "MainCode"

Start:
  ; Wir müssen nicht im Einzelnen wissen, was die init_snes-Routine macht.
  ;  Uns sollte nur klar sein, dass nach der Abarbeitung der Routine ...
  ;
  ;   -der Speicher des SNES gelöscht / genullt ist
  ;   -die Routine setzt den 'native mode' des 65816. Der 6502-Emulationsmodus
  ;     ist also abgeschaltet
  ;   -der Dezimalmodus ist abgeschaltet, wir rechnen also binär 
  ;   -Es initialisiert den Stapelzeiger auf Adresse $1FFF (von 'bank' 0).
  ;   -Es wird 'data bank' = 'program bank' gesetzt
  ;   -die 'Direct Page' wird auf Adresse $0000 (in 'bank' 0) gesetzt
  ;   -das SNES wird auf ein SLOWROM initialisiert
  ;   -Alle Interrupts sind eingeschaltet, also nicht maskiert.
  init_snes            

  ; Farbpalette laden und via DMA in den CGRAM kopieren
  load_palette bg_palette, 0, 4

  ; Die einzelnen Tiles (druckbaren Zeichen) via DMA in den VRAM laden
  ;  Es sind insgesamt 256 tiles, also insgesamt 4096 Bytes
  load_block_to_vram tiles, $0000, $1000	

  ; VRAM auf 'byte access' einstellen
  lda #$00
  sta VIDEO_CONTROL

  ; Anfangsadresse des zu druckenden Textes.  
  ;  
  ; Hier: 3. Zeile, 3. Spalte
  ldx #$0442	
  stx VRAM_ADDRESS

  ; Zählvariable für den Offset der einzelnen Zeichen
  ldy #0

; Die folgende Schleife kopiert unseren Text 'auf den Bildschirm'
copyText:

  ; Lade das aktuelle Zeichen in den Akkumulator
  lda helloText, y
  ; Wenn das letzte Zeichen das '\0' war, dann sind wir fertig, also springe zu
  ;  done
  beq done

  ; Ansonsten speichere das zuletzt gelesene Zeichen im VRAM
  sta VRAM_DATA

  ; inkrementiere den Inhalt der Y-Registers für die indizierte Adressierung
  iny

  ; Wir sind noch nicht fertig, also müssen wir wider hoch springen, um das
  ;  nächste Zeichen zu lesen
  bne copyText    

done:
  ; Wir sind fertig (mit dem Kopieren des Texts). Also können wir nun den 
  ;  Bildschirm wieder einschalten
  jsr setup_video

; Fertig -> Endlosschleife
loop:
  jmp loop

; Unser Text, der auf dem Bildschirm dargestellt werden soll
helloText:
  .db "Hello",$20,"World",$00

;===============================================================================
; 'setup_video' -- Stellt den Video-Modus und die Register für die 'Tile'-Daten
;  ein.
;-------------------------------------------------------------------------------
; Eingabeparameter: /
;-------------------------------------------------------------------------------
; Ausgabeparameter: /
;-------------------------------------------------------------------------------
setup_video:
  ; callee save
  php

  ; Videomodus auf Modus 0 setzen. Dies bedeutet, dass wir insgesamt nur 4 
  ;  Hintergundfarben benutzen können 
  ; Des Weiteren wird hier festgelegt, dass wir 8x8 große 'Tiles' benutzen 
  lda #$00
  sta $2105           

  ; Setze den Offset / die Basisadresse des 1. Hintegrunds auf Adresse $0400
  ; Setze die Größe der TileMap auf 32x32-Zeichen
  lda #$04            
  sta $2107          

  ; (??)
  stz $210B           

  ; Aktiviere Hintegrund 1
  lda #$01           
  sta $212C

  ; Bildschirm einschalten (volle Helligkeit (100%)
  lda #SCREEN_ON & BG_BRIGHTNESS_100
  sta SCREEN_DISPLAY

  ; reverse callee save
  plp

  ; return from subroutine
  rts
;===============================================================================

.ENDS

; Die eigentlichen Kacheln (engl. tiles) in 'bank' 1 einbinden.
;  Diese Daten müssen noch in den VRAM kopiert werden
.BANK 1 SLOT 0
.ORG 0
.SECTION "TileData"

  .INCLUDE "tiles.inc"

.ENDS
