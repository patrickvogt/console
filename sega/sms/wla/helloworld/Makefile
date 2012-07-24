################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.sms obj/hello.o tools/chr2asciiart.exe \
  tools/asciiart2chr.exe bin/pattern_table_1.chr

ASSEMBLER = wla-z80 # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = wlalink # Dieser 'linkt' unsere Objektdateien in ein ROM
EMU = sms  # Legt den SMS-Emulator fest, mit dem wir unser ROM testen
#DEPLOYER = mega-usb #
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
all: dist/hello.sms

dist/hello.sms: tools bin/pattern_table_1.chr src/main.asm src/sdsc.inc src/vdp.inc ./hello.link
	 cd src && $(ASSEMBLER) -vo main.asm ../obj/hello.o && cd ..
	 $(LINKER) -vr ./hello.link dist/hello.sms

# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/hello.sms

# erstellt ausführbare Hilfs-Programme
tools: tools/chr2asciiart tools/asciiart2chr

# erstellt ausführbares Hilfs-Programm 'chr2asciiart'
tools/chr2asciiart: tools/chr2asciiart.c
	$(CC) $(CFLAGS) $< -o $@

# erstellt ausführbares Hilfs-Programm 'asciiart2chr'
tools/asciiart2chr: tools/asciiart2chr.c
	$(CC) $(CFLAGS) $< -o $@

# wandelt die ASCII-Tabelle in ein CHR-Rom um
bin/pattern_table_1.chr: res/pattern_table_1.txt
	tools/asciiart2chr $< $@

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
