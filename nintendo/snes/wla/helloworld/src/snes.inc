; Screen Display Register @ $2100
;   Bit 7   - 0 == Bildschirm eingeschaltet
;             1 == Bildschirm ausgeschaltet (No VBLANKS)
;   Bit 6-4 - immer 0
;   Bit 3-0 - Helligkeit des Bildschirms
;               0000 ==   0% Helligkeit
;               1111 == 100% Helligkeit
.DEFINE SCREEN_DISPLAY $2100

; Bitmaske, um die Registerbreite des Akkumulators auf 8 Bit zu setzen
.DEFINE ACCU_8BIT %00100000

; Hintergrundfarbe des Bildschirms (die unteren 8 Bits)
.DEFINE BG_COLOR_LOW %00000000
; Hintegrdungfarbe des Bildschirms (die oberen 8 Bits)
.DEFINE BG_COLOR_HIGH %00000000

; Bitmaske, um den Bildschirm abzuschalten
.DEFINE SCREEN_OFF %10000000

; Bitmaske um den Bildschirm einzuschalten 
;  Achtung: Muss mit AND verknüpft werden um garantiert Bit 7 zu löschen
.DEFINE SCREEN_ON %01111111

; Bitmaske, um die Helligkeit des Hintergrunds zu setzen
.DEFINE BG_BRIGHTNESS_100 %00001111

; Bitmaske, um die Negativ-Flagge im P-Resister zu setzen / zu löschen
.DEFINE P_NEGATIVE %10000000

; Bitmaske, um die Überlauf-Flagge im P-Resister zu setzen / zu löschen
.DEFINE P_OVERFLOW %01000000

; Bitmaske, um die Registerbreite des Akkumulators im P-Resister zu setzen / 
;  zu löschen
;
; Hinweis: Diese Operation ist nur im 'native mode' zulässig
.DEFINE P_N_ACCU_REGISTER_SIZE %00100000

; Bitmaske, um die Registerbreite des X-/Y-Registers im P-Resister zu setzen / 
;  zu löschen
;
; Hinweis: Diese Operation ist nur im 'native mode' zulässig
.DEFINE P_N_INDEX_REGISTER_SIZE %00010000

; Bitmaske, um die Break-Flagge im P-Resister zu setzen / 
;  zu löschen
;
; Hinweis: Diese Operation ist nur im '6502-emulation mode' zulässig
.DEFINE P_E_BREAK %00010000

; Bitmaske, um die Dezimal-Flagge im P-Resister zu setzen / 
;  zu löschen
.DEFINE P_DECIMAL %00001000

; Bitmaske, um die Interrupt-Flagge im P-Resister zu setzen / 
;  zu löschen
;
; Hinweis: Bei gesetzter Interrupt-Flagge, werden alle Unterbrechungen maskiert/
;  ignoriert (außer NMI)
.DEFINE P_INTERRUPT %00000100

; Bitmaske, um die Zero-Flagge im P-Resister zu setzen / 
;  zu löschen
.DEFINE P_ZERO %00000010

; Bitmaske, um die Übertrag-Flagge im P-Resister zu setzen / 
;  zu löschen
.DEFINE P_CARRY %00000001

; In diesem Register wird die Adresse für zuküntige Schreiboperationen in 
;  den CGRAM definiert
.DEFINE CGRAM_ADDRESS $2121

; Über dieses Register können Daten (jeweils 2 Byte) in den CGRAM geschrieben 
;  werden.
;  
; Hinweis: Zuerst muss das Low-Byte geschrieben werden, dann das High-Byte
.DEFINE CGRAM_DATA $2122

; Mithilfe dieses Registers können die 8 DMA-Kanäle eingeschaltet / aktiviert
;  werden.
;
; Kanal 0 aktivieren == Bit 0 auf 1 setzen
; Kanal 1 aktivieren == Bit 1 auf 1 setzen
; ...
; Kanal 7 aktivieren == Bit 7 auf 1 setzen
.DEFINE DMA_CONTROL $420B

; Bitmaske, um den 'DMA Channel 0' zu aktivieren. Diese Bitmaske muss auf das 
;  'DMA Control Register' angewendet werden
.DEFINE DMA_CH0_ENABLE %00000001

; Dieses Register legt grundsätzliche Eingeschaften vom DMA-Kanal 0 fest.
;  Bspw. können die folgenden Eigenschaften über dieses Register eingestellt 
;  werden:
;   -Die Richtung des DMA-Transfers (PPU-Register ?<->? CPU-Memory)
;   -ob ein 'fixed' DMA-Transfer durchgeführt werden soll. 
;     Durch ein 'fixed' DMA-Transfer wird die DMA-Quelladresse nicht 
;     inkrementiert (glaube ich zumindest??)
;   -der Transfer-Mode von Kanal 0
.DEFINE DMA_CH0_CONTROL $4300

; Legt die Zieladresse des 0. DMA-Kanals fest.
;
; Die absolute Zieladresse ergibt sich durch das Prefix $21 und diesem Byte
;
; Beispiel-Inhalt dieses Registers $18, dann ist die tatsächliche Zieladresse
;  $2118
.DEFINE DMA_CH0_DESTINATION $4301

; 16-Bit Quelladresse für DMA-Kanal 0
.DEFINE DMA_CH0_SOURCE_OFFSET $4302

; Quell-Bank für DMA-Kanal 0
.DEFINE DMA_CH0_SOURCE_BANK $4304

; Anzahl der Bytes, die über den DMA-Kanal 0 übertragen werden sollen
.DEFINE DMA_CH0_NUMBER_OF_BYTES $4305

.DEFINE VIDEO_CONTROL $2115

; Register um die Adresse (genauer das Low-Byte) der VRAM port-Adresse zu setzen
.DEFINE VRAM_ADDRESS_L $2116

; Register um die Adresse (genauer das High-Byte) der VRAM port-Adresse zu 
;  setzen
.DEFINE VRAM_ADDRESS_H $2117

; Für den 'word access' einfach nochmal ein besserer symbolischer Name ohne dem
;  Suffix '_L'
.DEFINE VRAM_ADDRESS $2116

; Register um Daten in den VRAM zu schreiben bzw. vom VRAM zu lesen
.DEFINE VRAM_DATA $2118

; Register um Daten in den VRAM zu schreiben bzw. vom VRAM zu lesen
;  Explizite Low-Byte-Variante dieses symbolischen Namens
.DEFINE VRAM_DATA_L $2118

; Register um Daten in den VRAM zu schreiben bzw. vom VRAM zu lesen
;  Explizite High-Byte-Variante dieses symbolischen Namens
.DEFINE VRAM_DATA_H $2119
