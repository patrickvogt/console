################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.gg obj/hello.o tools/chr2asciiart.exe \
  tools/asciiart2chr.exe bin/pattern_table_1.chr

ASSEMBLER = wla-z80 # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = wlalink # Dieser 'linkt' unsere Objektdateien in ein ROM
EMU = gg  # Legt den Emulator fest, mit dem wir unser ROM testen
#DEPLOYER = # Legt das Programm fest, um das ROM auf eine Karte zu übertragen
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
all: dist/hello.gg

dist/hello.gg: tools bin/pattern_table_1.chr src/hello.asm ./hello.link
	 cd src && $(ASSEMBLER) -vo hello.asm ../obj/hello.o && cd ..
	 $(LINKER) -rv ./hello.link dist/hello.gg
  
# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/hello.gg
  
#deploy: all
#	$(DEPLOYER) dist/hello.gg

# erstellt ausführbare Hilfs-Programme
tools: tools/chr2asciiart tools/asciiart2chr

# erstellt ausführbares Hilfs-Programm 'chr2asciiart'
tools/chr2asciiart: tools/chr2asciiart.c
	$(CC) $(CFLAGS) $< -o $@

# erstellt ausführbares Hilfs-Programm 'asciiart2chr'
tools/asciiart2chr: tools/asciiart2chr.c
	$(CC) $(CFLAGS) $< -o $@

# wandelt die ASCII-Tabelle in ein CHR-ROM um
bin/pattern_table_1.chr: res/pattern_table_1.txt
	tools/asciiart2chr $< $@

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
