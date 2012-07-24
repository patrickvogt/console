################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.nes obj/main.o tools/chr2asciiart.exe \
  tools/asciiart2chr.exe bin/pattern_table_1.chr bin/pattern_table_2.chr

ASSEMBLER = wla-6502 # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = wlalink # Dieser 'linkt' unsere Objektdateien in ein ROM
EMU = nes  # Legt den Emulator fest, mit dem wir unser ROM testen
DEPLOYER = # Legt das Programm fest, um das ROM auf eine Karte zu übertragen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
ASSFLAGS = # Argumente für den Assembler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run clean tools deploy

# Standard-Target wird automatisch bei einem 'make' ausgeführt
all: tools bin/pattern_table_1.chr bin/pattern_table_2.chr dist/hello.nes

# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/hello.nes
  
#deploy: all
#	$(DEPLOYER) dist/hello.nes
  
# erstellt ausführbare Programme der Hilfswerkzeuge
tools: tools/chr2asciiart tools/asciiart2chr

# erstellt ausführbare Programme der Hilfswerkzeuge
tools/chr2asciiart: tools/chr2asciiart.c
	$(CC) $(CFLAGS) $< -o $@

# erstellt ausführbare Programme der Hilfswerkzeuge
tools/asciiart2chr: tools/asciiart2chr.c
	$(CC) $(CFLAGS) $< -o $@

# wandelt die ASCII-Tabelle in ein CHR-ROM um
bin/pattern_table_1.chr: res/pattern_table_1.txt
	tools/asciiart2chr $< $@

# wandelt die ASCII-Tabelle in ein CHR-ROM um
bin/pattern_table_2.chr: res/pattern_table_2.txt
	tools/asciiart2chr $< $@

dist/hello.nes: src/main.asm src/nes.inc src/nes_memory.inc ./hello.link
	 cd src && $(ASSEMBLER) -vo main.asm ../obj/main.o && cd ..
	 $(LINKER) -vr ./hello.link dist/hello.nes
   
# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
