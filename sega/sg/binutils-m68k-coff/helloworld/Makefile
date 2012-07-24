################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.bin memory.map \
 obj/sega.o obj/main.o obj/tiles.o \
 obj/z80driver.o bin/a.out Gens.cfg language.dat

ASSEMBLER = m68k-coff-as # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = m68k-coff-ld # Dieser 'linkt' unsere Objektdateien in ein ROM
OBJCOPY = m68k-coff-objcopy # Das ROM in ein gültiges SG-ROM umwandeln
EMU = sg  # Legt den Emulator fest, mit dem wir unser Programm testen
DEPLOYER = mega-usb # 
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
ASSFLAGS = -m68000 --register-prefix-optional --bitwise-or # Argumente für den Assembler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run clean deploy

all: dist/hello.bin

# Hier ist die Reihenfolge der Prerequisits(?) wichtig!
dist/hello.bin: obj/sega.o obj/z80driver.o obj/main.o obj/tiles.o 
	 $(LINKER) -Tsg.link -Map memory.map $^ -o bin/a.out
	 $(OBJCOPY) -I coff-m68k -O binary bin/a.out $@
   
obj/sega.o: src/sega.asm
	 $(ASSEMBLER) $(ASSFLAGS) -o $@ $<
   
obj/z80driver.o: src/z80driver.asm
	 $(ASSEMBLER) $(ASSFLAGS) -o $@ $<
   
obj/main.o: src/main.asm
	 $(ASSEMBLER) $(ASSFLAGS) -o $@ $<
   
obj/tiles.o: src/tiles.asm
	 $(ASSEMBLER) $(ASSFLAGS) -o $@ $<

# startet das erstellte NES-Rom im Emulator
run: all
	$(EMU) dist/hello.bin
  
#deploy: all
#	$(DEPLOYER) dist/hello.bin

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
  