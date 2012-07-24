; Speicherarchitektur des NES definieren
.MEMORYMAP
; Standard-Slot ist die Zero-Page
DEFAULTSLOT 1
; iNES-Header
SLOTSIZE $10
SLOT 0 $0
; Zero-Page
SLOTSIZE $100
SLOT 1 $0
; RAM
SLOTSIZE $500
SLOT 2 $300 
; UPPER BANK PRG-ROM (16KB)
SLOTSIZE $4000
SLOT 3 $C000
; CHR-ROM (8KB)
SLOTSIZE $2000
SLOT 4 $0
.ENDME

; Unser kompiliertes NES-ROM besteht aus 
;  -1x iNES-Header
;  -1x PRG-ROM (16KB)
;  -1x CHR-ROM (8KB)
.ROMBANKMAP
; Insgesamt 3 'banks' (siehe oben)
BANKSTOTAL 3
; iNES-Header
BANKSIZE $10
BANKS 1
; PRG-ROM
BANKSIZE $4000 
BANKS 1
; CHR-ROM
BANKSIZE $2000 
BANKS 1
.ENDRO
