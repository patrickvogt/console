;===============================================================================
; Makros
;===============================================================================

;===============================================================================
; 'load_palette' -- Lädt die Farbpalette in den CGRAM
;-------------------------------------------------------------------------------
; Eingabeparameter:  SRC_ADDR -- 24 Bit-Adresse der Quell-Farbpalette
;                    START    -- Nummer der ersten zu belegenden Farbe, 
;                    SIZE     -- Anzahl an Farben, die kopiert werden sollen
;-------------------------------------------------------------------------------
; Ausgabeparameter: /
;-------------------------------------------------------------------------------
; modifiziert: A, X
; Voraussetzungen: mem/A = 8 Bit-Modus, X/Y = 16 Bit-Modus
;-------------------------------------------------------------------------------
.MACRO load_palette
  ; Lade die übergebene Nummer der ersten zu belegenden Farbe und speichere
  ;  diese in das CGRAM-Adressregister
  lda #\2
  sta CGRAM_ADDRESS       

  ; Lade den Akkumulator mit der Quell-Bank der Farbpalette
  ;
  ; Hinweis: Der Operator ':' gibt die Bank einer gegebenen Adresse zurück
  lda #:\1       
  ; Lade das X-Register mit der Quelladresse der Farbpalette
  ldx #\1         
  ; Lade das Y-Register mit der Anzahl der Bytes der Farbpalette. 
  ;  Jede Farbe besteht aus jeweils 2 Byte, also ist 'Anzahl der Farben'*2 Byte
  ;  die gewünschte Größe des zu kopierenden Bereichs
  ldy #(\3 * 2)
  ; Alles vorbereitet => den eigentlichen DMA-Transfer führen wir in einem
  ;  Unterprogramm aus
  jsr dma_palette
.ENDM

;===============================================================================
; 'load_block_to_vram' -- Kopiert die 'Tile'-Daten in den VRAM
;-------------------------------------------------------------------------------
; Eingabeparameter:  SRC_ADDR -- 24 Bit-Adresse der zu kopierenden 'Tile'-Daten
;                    DEST     -- VRAM-Adresse, an der mit dem Schreiben der 
;                                 Daten begonnen werden soll
;                    SIZE     -- Anzahl der Bytes, die kopiert werden sollen
;-------------------------------------------------------------------------------
; Ausgabeparameter: /
;-------------------------------------------------------------------------------
; modifiziert: A, X, Y
; Voraussetzungen: mem/A = 8 Bit-Modus, X/Y = 16 Bit-Modus
;-------------------------------------------------------------------------------
.MACRO load_block_to_vram
  ; 'VRAM port' auf 'word access' stellen,
  ;   Erst nach dem Schreibvorgang nach Adresse $2119 wird die Adresse im VRAM
  ;   inkrementiert (??) 
  lda #%10000000
  sta VIDEO_CONTROL

  ; Initialisieren die VRAM port-Adresse mit der übergebenen Adresse
  ldx #\2         
  stx VRAM_ADDRESS
  ; Lade die Quellbank für den DMA-Transfer in den Akkumulator
  lda #:\1        
  ; Lade die Quelladresse für den DMA-Transfer in das X-Register
  ldx #\1         
  ; Lade die Anzahl der zu kopierenden Bytes in das Y-Register
  ldy #\3       

  ; Alles vorbereitet => den eigentlichen DMA-Transfer führen wir in einem
  ;  Unterprogramm aus
  jsr load_vram
.ENDM

;===============================================================================
; Routinen
;===============================================================================

.BANK 0
.ORG 0
.SECTION "LoadVRAMCode" SEMIFREE

;===============================================================================
; 'load_vram' -- Lädt Daten (von der übergebeben Adresse) in den VRAM
;-------------------------------------------------------------------------------
; Eingabeparameter:  A:X  -- 24 Bit-Adresse der zu übertragenden Daten
;                    Y    -- Anzahl der Bytes, die kopiert werden sollen
;-------------------------------------------------------------------------------
; Ausgabeparameter: /
;-------------------------------------------------------------------------------
; modifiziert: /
;-------------------------------------------------------------------------------
; Hinweis:  Es wird angenommen, dass die VRAM-Adresse zuvor richtig gesetzt
;            wurde!
;           Es wird angenommen, dass die Registerbreite des X-/Y-Registers auf
;            16 Bit eingestellt ist
;-------------------------------------------------------------------------------
load_vram:
  ; callee save
  pha
  phx
  phy
  phb
  php         

  ; Setze Akkumulator in 8 Bit-Modus
  sep #P_N_ACCU_REGISTER_SIZE

  ; Lege die Quelladresse für den DMA-Transfer fest
  stx DMA_CH0_SOURCE_OFFSET   
  sta DMA_CH0_SOURCE_BANK  

  ; Lege die Anzahl der zu kopierenden Bytes (für den kommenden DMA-Transfer) 
  ;  fest
  sty DMA_CH0_NUMBER_OF_BYTES   

  ; Lege den Transfer-Mode des DMA-Transfers fest (hier auf: 16 Bit ('word') und
  ;  die Inkrementierung der DMA-Quelladresse
  lda #$01
  sta DMA_CH0_CONTROL
  
  ; Sete das Zielregister des DMA-Transfers: Das VRAM Data-Register
  lda #(VRAM_DATA & $00FF)
  sta DMA_CH0_DESTINATION

  ; Aktiviere DMA-Channel 1
  lda #DMA_CH0_ENABLE
  sta DMA_CONTROL

  ; reverse callee save
  plp
  plb
  ply
  plx
  pla

  ; return from subroutine
  rts
;===============================================================================

.ENDS

.BANK 0
.ORG 0
.SECTION "DMAPaletteCode" SEMIFREE

;===============================================================================
; 'dma_palette' -- Initialisiert die gesamte Farbpalette via DMA-Transfer
;-------------------------------------------------------------------------------
; Eingabeparameter:   A:X  -- 24-Bit Adresse der Quell-Farbpalette
;                     Y    -- Anzahl der Bytes, die via DMA transferiert werden
;                              sollen
;-------------------------------------------------------------------------------
; Ausgabeparameter: /
;-------------------------------------------------------------------------------
; modifiziert: /
;-------------------------------------------------------------------------------
; Hinweis:  Es wird angenommen, dass die CGRAM-Adresse zuvor richtig gesetzt
;            wurde!
;-------------------------------------------------------------------------------
dma_palette:
  ; callee save
  pha
  phx
  phb
  php

  ; Setze Akkumulator in 8 Bit-Modus
  sep #P_N_ACCU_REGISTER_SIZE

  ; Lege die DMA-Quelladresse fest
  stx DMA_CH0_SOURCE_OFFSET
  ; Lege die DMA-Quellbank fest
  sta DMA_CH0_SOURCE_BANK
  ; Lege fest, wie viel Bytes via DMA übertragen / kopiert werden sollen
  sty DMA_CH0_NUMBER_OF_BYTES

  ; Setze dem DMA-Transfermodus auf 1 Byte und die Quelladresse soll während des
  ;  DMA-Transfers inkrementiert werden
  stz DMA_CH0_CONTROL
  ; Lege das Zielregister des DMA-Transfers fest
  ;  Da wir die Farbpalette initialisieren wollen, müssen wir in den CGRAM via
  ;  Register $2122 schreiben
  lda #(CGRAM_DATA & $00FF)    
  sta DMA_CH0_DESTINATION
  lda #DMA_CH0_ENABLE    ; Initiate DMA transfer
  sta DMA_CONTROL

  ; reverse callee save
  plp         
  plb
  plx
  pla

  ; return from subroutine
  rts
;===============================================================================

.ENDS
