;*
;* GBHW.INC - Gameboy Hardware definitions for GALP
;*
;* by GABY. Inspired from work done by Carsten Sorensen & others.
;*
;* V1.0 - The original version
;*

.DEFINE _HW $FF00

.DEFINE _VRAM $8000 ; $8000->$A000
.DEFINE _SCRN0 $9800 ; $9800->$9BFF
.DEFINE _SCRN1 $9C00 ; $9C00->$9FFF
.DEFINE _RAM $C000 ; $C000->$E000
.DEFINE _HRAM $F800 ; $F800->$FFFE
.DEFINE _OAMRAM $FE00 ; $FE00->$FE9F
.DEFINE _AUD3WAVERAM $FF30 ; $FF30->$FF3F

; --
; -- OAM flags
; --

.DEFINE OAMF_PRI %10000000 ; Priority
.DEFINE OAMF_YFLIP %01000000 ; Y flip
.DEFINE OAMF_XFLIP %00100000 ; X flip
.DEFINE OAMF_PAL0 %00000000 ; Palette number; 0,1
.DEFINE OAMF_PAL1 %00010000 ; Palette number; 0,1

;***************************************************************************
;*
;* Custom registers
;*
;***************************************************************************

; --
; -- P1 ($FF00)
; -- Register for reading joy pad info.    (R/W)
; --
.DEFINE rP1  $FF00

.DEFINE P1F_5     %00100000 ; P15 out port
.DEFINE P1F_4     %00010000 ; P14 out port
.DEFINE P1F_3     %00001000 ; P13 in port
.DEFINE P1F_2     %00000100 ; P12 in port
.DEFINE P1F_1     %00000010 ; P11 in port
.DEFINE P1F_0     %00000001 ; P10 in port


; --
; -- LCDC ($FF40)
; -- LCD Control (R/W)
; --
.DEFINE rLCDC  $FF40

.DEFINE LCDCF_OFF       %00000000 ; LCD Control Operation
.DEFINE LCDCF_ON        %10000000 ; LCD Control Operation
.DEFINE LCDCF_WIN9800   %00000000 ; Window Tile Map Display Select
.DEFINE LCDCF_WIN9C00   %01000000 ; Window Tile Map Display Select
.DEFINE LCDCF_WINOFF    %00000000 ; Window Display
.DEFINE LCDCF_WINON     %00100000 ; Window Display
.DEFINE LCDCF_BG8800    %00000000 ; BG & Window Tile Data Select
.DEFINE LCDCF_BG8000    %00010000 ; BG & Window Tile Data Select
.DEFINE LCDCF_BG9800    %00000000 ; BG Tile Map Display Select
.DEFINE LCDCF_BG9C00    %00001000 ; BG Tile Map Display Select
.DEFINE LCDCF_OBJ8      %00000000 ; OBJ Construction
.DEFINE LCDCF_OBJ16     %00000100 ; OBJ Construction
.DEFINE LCDCF_OBJOFF    %00000000 ; OBJ Display
.DEFINE LCDCF_OBJON     %00000010 ; OBJ Display
.DEFINE LCDCF_BGOFF     %00000000 ; BG Display
.DEFINE LCDCF_BGON      %00000001 ; BG Display
; "Window Character Data Select" follows BG


; --
; -- STAT ($FF41)
; -- LCDC Status   (R/W)
; --
.DEFINE rSTAT  $FF41

.DEFINE STATF_LYC       %01000000 ; LYCLY Coincidence (Selectable)
.DEFINE STATF_MODE10    %00100000 ; Mode 10
.DEFINE STATF_MODE01    %00010000 ; Mode 01 (V-Blank)
.DEFINE STATF_MODE00    %00001000 ; Mode 00 (H-Blank)
.DEFINE STATF_LYCF      %00000100 ; Coincidence Flag
.DEFINE STATF_HB        %00000000 ; H-Blank
.DEFINE STATF_VB        %00000001 ; V-Blank
.DEFINE STATF_OAM       %00000010 ; OAM-RAM is used by system
.DEFINE STATF_LCD       %00000011 ; Both OAM and VRAM used by system
.DEFINE STATF_BUSY      %00000010 ; When set, VRAM access is unsafe


; --
; -- SCY ($FF42)
; -- Scroll Y (R/W)
; --
.DEFINE rSCY   $FF42


; --
; -- SCY ($FF43)
; -- Scroll X (R/W)
; --
.DEFINE rSCX   $FF43


; --
; -- LY ($FF44)
; -- LCDC Y-Coordinate (R)
; --
; -- Values range from 0->153. 144->153 is the VBlank period.
; --
.DEFINE rLY  $FF44


; --
; -- LYC ($FF45)
; -- LY Compare (R/W)
; --
; -- When LYLYC, STATF_LYCF will be set in STAT
; --
.DEFINE rLYC   $FF45


; --
; -- DMA ($FF46)
; -- DMA Transfer and Start Address (W)
; --
.DEFINE rDMA   $FF46


; --
; -- BGP ($FF47)
; -- BG Palette Data (W)
; --
; -- Bit 7-6 - Intensity for %11
; -- Bit 5-4 - Intensity for %10
; -- Bit 3-2 - Intensity for %01
; -- Bit 1-0 - Intensity for %00
; --
.DEFINE rBGP   $FF47


; --
; -- OBP0 ($FF48)
; -- Object Palette 0 Data (W)
; --
; -- See BGP for info
; --
.DEFINE rOBP0  $FF48


; --
; -- OBP1 ($FF49)
; -- Object Palette 1 Data (W)
; --
; -- See BGP for info
; --
.DEFINE rOBP1  $FF49


; --
; -- SB ($FF01)
; -- Serial Transfer Data (R/W)
; --
.DEFINE rSB  $FF01

; --
; -- SC ($FF02)
; -- Serial I/O Control (R/W)
; --
.DEFINE rSC  $FF02

; --
; -- DIV ($FF04)
; -- Divider register (R/W)
; --
.DEFINE rDIV  $FF04


; --
; -- TIMA ($FF05)
; -- Timer counter (R/W)
; --
.DEFINE rTIMA  $FF05


; --
; -- TMA ($FF06)
; -- Timer modulo (R/W)
; --
.DEFINE rTMA  $FF06


; --
; -- TAC ($FF07)
; -- Timer control (R/W)
; --
.DEFINE rTAC  $FF07

.DEFINE TACF_START    %00000100
.DEFINE TACF_STOP     %00000000
.DEFINE TACF_4KHZ     %00000000
.DEFINE TACF_16KHZ    %00000011
.DEFINE TACF_65KHZ    %00000010
.DEFINE TACF_262KHZ   %00000001


; --
; -- IF ($FF0F)
; -- Interrupt Flag (R/W)
; --
; -- IE ($FFFF)
; -- Interrupt Enable (R/W)
; --
.DEFINE rIF  $FF0F
.DEFINE rIE  $FFFF

.DEFINE IEF_HILO      %00010000 ; Transition from High to Low of Pin number P10-P13
.DEFINE IEF_SERIAL    %00001000 ; Serial I/O transfer end
.DEFINE IEF_TIMER     %00000100 ; Timer Overflow
.DEFINE IEF_LCDC      %00000010 ; LCDC (see STAT)
.DEFINE IEF_VBLANK    %00000001 ; V-Blank


; --
; -- WY ($FF4A)
; -- Window Y Position (R/W)
; --
; -- 0 < WY < 143
; --
.DEFINE rWY  $FF4A


; --
; -- WX ($FF4B)
; -- Window X Position (R/W)
; --
; -- 7 < WX < 166
; --
.DEFINE rWX  $FF4B


;***************************************************************************
;*
;* Sound control registers
;*
;***************************************************************************

; --
; -- AUDVOL/NR50 ($FF24)
; -- Channel control / ON-OFF / Volume (R/W)
; --
; -- Bit 7   - Vin->SO2 ON/OFF (Vin??)
; -- Bit 6-4 - SO2 output level (volume) (# 0-7)
; -- Bit 3   - Vin->SO1 ON/OFF (Vin??)
; -- Bit 2-0 - SO1 output level (volume) (# 0-7)
; --
.DEFINE rNR50  $FF24
.DEFINE rAUDVOL	 rNR50


; --
; -- AUDTERM/NR51 ($FF25)
; -- Selection of Sound output terminal (R/W)
; --
; -- Bit 7   - Output sound 4 to SO2 terminal
; -- Bit 6   - Output sound 3 to SO2 terminal
; -- Bit 5   - Output sound 2 to SO2 terminal
; -- Bit 4   - Output sound 1 to SO2 terminal
; -- Bit 3   - Output sound 4 to SO1 terminal
; -- Bit 2   - Output sound 3 to SO1 terminal
; -- Bit 1   - Output sound 2 to SO1 terminal
; -- Bit 0   - Output sound 0 to SO1 terminal
; --
.DEFINE rNR51  $FF25
.DEFINE rAUDTERM	 rNR51


; --
; -- AUDENA/NR52 ($FF26)
; -- Sound on/off (R/W)
; --
; -- Bit 7   - All sound on/off (sets all audio regs to 0!)
; -- Bit 3   - Sound 4 ON flag (doesn't work!)
; -- Bit 2   - Sound 3 ON flag (doesn't work!)
; -- Bit 1   - Sound 2 ON flag (doesn't work!)
; -- Bit 0   - Sound 1 ON flag (doesn't work!)
; --
.DEFINE rNR52  $FF26
.DEFINE rAUDENA	 rNR52


;***************************************************************************
;*
;* SoundChannel #1 registers
;*
;***************************************************************************

; --
; -- AUD1SWEEP/NR10 ($FF10)
; -- Sweep register (R/W)
; --
; -- Bit 6-4 - Sweep Time
; -- Bit 3   - Sweep Increase/Decrease
; --           0: Addition    (frency increases???)
; --           1: Subtraction (frency increases???)
; -- Bit 2-0 - Number of sweep shift (# 0-7)
; -- Sweep Time: (n*7.8ms)
; --
.DEFINE rNR10  $FF10
.DEFINE rAUD1SWEEP	 rNR10


; --
; -- AUD1LEN/NR11 ($FF11)
; -- Sound length/Wave pattern duty (R/W)
; --
; -- Bit 7-6 - Wave Pattern Duty (00:12.5% 01:25% 10:50% 11:75%)
; -- Bit 5-0 - Sound length data (# 0-63)
; --
.DEFINE rNR11  $FF11
.DEFINE rAUD1LEN	 rNR11


; --
; -- AUD1ENV/NR12 ($FF12)
; -- Envelope (R/W)
; --
; -- Bit 7-4 - Initial value of envelope
; -- Bit 3   - Envelope UP/DOWN
; --           0: Decrease
; --           1: Range of increase
; -- Bit 2-0 - Number of envelope sweep (# 0-7)
; --
.DEFINE rNR12  $FF12
.DEFINE rAUD1ENV	 rNR12


; --
; -- AUD1LOW/NR13 ($FF13)
; -- Frency lo (W)
; --
.DEFINE rNR13  $FF13
.DEFINE rAUD1LOW	 rNR13


; --
; -- AUD1HIGH/NR14 ($FF14)
; -- Frency hi (W)
; --
; -- Bit 7   - Initial (when set, sound restarts)
; -- Bit 6   - Counter/consecutive selection
; -- Bit 2-0 - Frency's higher 3 bits
; --
.DEFINE rNR14  $FF14
.DEFINE rAUD1HIGH	 rNR14


;***************************************************************************
;*
;* SoundChannel #2 registers
;*
;***************************************************************************

; --
; -- AUD2LEN/NR21 ($FF16)
; -- Sound Length; Wave Pattern Duty (R/W)
; --
; -- see AUD1LEN for info
; --
.DEFINE rNR21  $FF16
.DEFINE rAUD2LEN	 rNR21


; --
; -- AUD2ENV/NR22 ($FF17)
; -- Envelope (R/W)
; --
; -- see AUD1ENV for info
; --
.DEFINE rNR22  $FF17
.DEFINE rAUD2ENV	 rNR22


; --
; -- AUD2LOW/NR23 ($FF18)
; -- Frency lo (W)
; --
.DEFINE rNR23  $FF18
.DEFINE rAUD2LOW	 rNR23


; --
; -- AUD2HIGH/NR24 ($FF19)
; -- Frency hi (W)
; --
; -- see AUD1HIGH for info
; --
.DEFINE rNR24  $FF19
.DEFINE rAUD2HIGH	 rNR24


;***************************************************************************
;*
;* SoundChannel #3 registers
;*
;***************************************************************************

; --
; -- AUD3ENA/NR30 ($FF1A)
; -- Sound on/off (R/W)
; --
; -- Bit 7   - Sound ON/OFF (1ON,0OFF)
; --
.DEFINE rNR30  $FF1A
.DEFINE rAUD3ENA	 rNR30


; --
; -- AUD3LEN/NR31 ($FF1B)
; -- Sound length (R/W)
; --
; -- Bit 7-0 - Sound length
; --
.DEFINE rNR31  $FF1B
.DEFINE rAUD3LEN	 rNR31


; --
; -- AUD3LEVEL/NR32 ($FF1C)
; -- Select output level
; --
; -- Bit 6-5 - Select output level
; --           00: 0/1 (mute)
; --           01: 1/1
; --           10: 1/2
; --           11: 1/4
; --
.DEFINE rNR32  $FF1C
.DEFINE rAUD3LEVEL	 rNR32


; --
; -- AUD3LOW/NR33 ($FF1D)
; -- Frency lo (W)
; --
; -- see AUD1LOW for info
; --
.DEFINE rNR33  $FF1D
.DEFINE rAUD3LOW	 rNR33


; --
; -- AUD3HIGH/NR34 ($FF1E)
; -- Frency hi (W)
; --
; -- see AUD1HIGH for info
; --
.DEFINE rNR34  $FF1E
.DEFINE rAUD3HIGH	 rNR34


; --
; -- AUD4LEN/NR41 ($FF20)
; -- Sound length (R/W)
; --
; -- Bit 5-0 - Sound length data (# 0-63)
; --
.DEFINE rNR41  $FF20
.DEFINE rAUD4LEN	 rNR41


; --
; -- AUD4ENV/NR42 ($FF21)
; -- Envelope (R/W)
; --
; -- see AUD1ENV for info
; --
.DEFINE rNR42  $FF21
.DEFINE rAUD4ENV	 rNR42


; --
; -- AUD4POLY/NR42 ($FF22)
; -- Polynomial counter (R/W)
; --
; -- Bit 7-4 - Selection of the shift clock frency of the (scf)
; --           polynomial counter (0000-1101)
; --           freqdrf*1/2^scf (not sure)
; -- Bit 3 -   Selection of the polynomial counter's step
; --           0: 15 steps
; --           1: 7 steps
; -- Bit 2-0 - Selection of the dividing ratio of frencies (drf)
; --           000: f/4   001: f/8   010: f/16  011: f/24
; --           100: f/32  101: f/40  110: f/48  111: f/56  (f4.194304 Mhz)
; --
.DEFINE rNR42_2  $FF22
.DEFINE rAUD4POLY	 rNR42_2


; --
; -- AUD4GO/NR43 ($FF23)
; -- (has wrong name and value (ff30) in Dr.Pan's doc!)
; --
; -- Bit 7 -   Inital
; -- Bit 6 -   Counter/consecutive selection
; --
.DEFINE rNR43  $FF23
.DEFINE rAUD4GO	 rNR43	; silly name!

;***************************************************************************
;*
;* Cart related
;*
;***************************************************************************

.DEFINE ROM_NOMBC          0
.DEFINE ROM_MBC1           1
.DEFINE ROM_MBC1_RAM       2
.DEFINE ROM_MBC1_RAM_BAT   3
.DEFINE ROM_MBC2           5
.DEFINE ROM_MBC2_BAT       6
.DEFINE ROM_NOMBC_RAM      8
.DEFINE ROM_NOMBC_RAM_BAT  9

.DEFINE ROM_SIZE_256KBIT   0
.DEFINE ROM_SIZE_512KBIT   1
.DEFINE ROM_SIZE_1M        2
.DEFINE ROM_SIZE_2M        3
.DEFINE ROM_SIZE_4M        4
.DEFINE ROM_SIZE_8M        5
.DEFINE ROM_SIZE_16M       6

.DEFINE ROM_SIZE_32KBYTE   0
.DEFINE ROM_SIZE_64KBYTE   1
.DEFINE ROM_SIZE_128KBYTE  2
.DEFINE ROM_SIZE_256KBYTE  3
.DEFINE ROM_SIZE_512KBYTE  4
.DEFINE ROM_SIZE_1MBYTE    5
.DEFINE ROM_SIZE_2MBYTE    6

.DEFINE RAM_SIZE_0KBIT    0
.DEFINE RAM_SIZE_16KBIT   1
.DEFINE RAM_SIZE_64KBIT   2
.DEFINE RAM_SIZE_256KBIT  3
.DEFINE RAM_SIZE_1MBIT    4

.DEFINE RAM_SIZE_0KBYTE    0
.DEFINE RAM_SIZE_2KBYTE    1
.DEFINE RAM_SIZE_8KBYTE    2
.DEFINE RAM_SIZE_32KBYTE   3
.DEFINE RAM_SIZE_128KBYTE  4

;***************************************************************************
;*
;* Keypad related
;*
;***************************************************************************

.DEFINE PADF_DOWN            $80
.DEFINE PADF_UP              $40
.DEFINE PADF_LEFT            $20
.DEFINE PADF_RIGHT           $10
.DEFINE PADF_START           $08
.DEFINE PADF_SELECT          $04
.DEFINE PADF_B               $02
.DEFINE PADF_A               $01

.DEFINE PA.DB_DOWN            $7
.DEFINE PA.DB_UP              $6
.DEFINE PA.DB_LEFT            $5
.DEFINE PA.DB_RIGHT           $4
.DEFINE PA.DB_START           $3
.DEFINE PA.DB_SELECT          $2
.DEFINE PA.DB_B               $1
.DEFINE PA.DB_A               $0

;***************************************************************************
;*
;* Screen related
;*
;***************************************************************************

.DEFINE SCRN_X       160 ; Width of screen in pixels
.DEFINE SCRN_Y       144 ; Height of screen in pixels
.DEFINE SCRN_X_B     20  ; Width of screen in bytes
.DEFINE SCRN_Y_B     18  ; Height of screen in bytes

.DEFINE SCRN_VX      256 ; Virtual width of screen in pixels
.DEFINE SCRN_VY      256 ; Virtual height of screen in pixels
.DEFINE SCRN_VX_B    32  ; Virtual width of screen in bytes
.DEFINE SCRN_VY_B    32  ; Virtual height of screen in bytes

.MACRO ROM_HEADER  
;*
;* Nintendo scrolling logo
;* (Code won't work on a real GameBoy)
;* (if next six lines are altered.)

;    0123456789ABCDEF
 .DB $CE,$ED,$66,$66,$CC,$0D,$00,$0B,$03,$73,$00,$83,$00,$0C,$00,$0D
 .DB $00,$08,$11,$1F,$88,$89,$00,$0E,$DC,$CC,$6E,$E6,$DD,$DD,$D9,$99
 .DB $BB,$BB,$67,$63,$6E,$0E,$EC,$CC,$DD,$DC,$99,$9F,$BB,$B9,$33,$3E

 .DB "EXAMPLE",0,0,0,0,0,0,0,0 ; Cart name - 15bytes
 .DB 0                         ; $143
 .DB 0,0                       ; $144 - Licensee code (not important)
 .DB 0                         ; $146 - SGB Support indicator
 .DB \1                        ; $147 - Cart type
 .DB \2                        ; $148 - ROM Size
 .DB \3                        ; $149 - RAM Size
 .DB 1                         ; $14a - Destination code
 .DB $33                       ; $14b - Old licensee code
 .DB 0                         ; $14c - Mask ROM version
 .DB 0                         ; $14d - Complement check (important)
 .DW 0                         ; $14e - Checksum (not important)

.ENDM
