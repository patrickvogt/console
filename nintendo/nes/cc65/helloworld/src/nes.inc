; Die hier enthaltenen Kommentare sind sehr knapp gehalten. Für detaillierte 
; Informationen empfehle ich das Dokument "NES System Architecture v2.4" 
; (http://fms.komkon.org/EMUL8/NES.html) von Marat Fayzullin. 

; Allgemeine Einstellungen der PPU, bspw. ob ein NMI-Interrupt bei einem VBLANK
; ausgelöst werden soll
PPUCTRL = $2000

; Bitmaske, um die "Pattern Table" der Sprites auf Adresse $1000 zu setzen
OBJ_1000 = $08 ; $08 = %0000 1000

; Bitmaske, um die "Pattern Table" des Hintergrundbildschirms auf Adresse $1000 
; zu setzen
BG_1000 = $10 ; $10 = %0001 0000

; Diese Bitmaske kann benutzt werden, um das VBLANK_NMI-Interrupt-Bit in PPUCTRL
; zu setzen
VBLANK_NMI = $80 ; $80 = %1000 0000

; Definiert u.a., ob der Bildschirm eingeschaltet ist, ob die Sprites 
;	eingeschaltet sind oder die Hintergundfarbe des Bildschirms
PPUMASK = $2001

; Bitmaske, um den Bildschirm einzuschalten
BG_ON = $0A ; $0A = %0000 1010

; Bitmaske, um die Sprites einzuschalten
OBJ_ON = $14 ; $14 = %0001 0100

; PPU-Statusregister kann u.a. Kollisionen zwischen Sprites feststellen oder
; es kann mithilfe eines spezielles Bit innerhalb dieser Speicherstelle 
; abgefragt werden, ob sich die PPU noch im VBLANK-Zustand befindet
PPUSTATUS = $2002

; Diese Speicherstelle beeinflusst das Scrolling des Bildschirms
PPUSCROLL = $2005

; Mithilfe dieser Adresse kann eine Art Zeiger, der auf eine Speicherstelle im
; PPU-Speicher zeigt, positioniert werden.
;	Nach jeder Schreiboperation wird dieser Zeiger dann automatisch hochgezählt.	
;
; Hinweis: Da eine Speicheradresse gewöhnlich aus 16 Bit besteht, muss zuerst
; das High-Byte an diese Adresse geschrieben werden, danach das Low-Byte.
;
; BSP: Der folgende Code setzt den 'PPU-Speicherzeiger' auf Adresse $2000.
;
; lda #$20 ; High-Byte der 16 Bit-Adresse
; ldx #$00 ; Low-Byte der 16 Bit-Adresse
; sta PPUADDR
; stx PPUADDR
PPUADDR = $2006

; Mithilfe dieser Adresse kann auf den PPU-Speicher zugegriffen werden (lesend 
; und schreibend). Der PPU-Speicherzeiger muss jedoch entsprechend vorher 
; mithilfe PPUADDR positioniert werden.
PPUDATA = $2007
