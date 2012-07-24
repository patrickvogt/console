################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.nes tools/chr2asciiart.exe tools/asciiart2chr.exe \
  bin/hello.prg bin/pattern_table_1.chr bin/pattern_table_2.chr bin/hello.chr \
  obj/main.o

ASSEMBLER = ca65 # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = ld65 # Dieser 'linkt' unsere Objektdateien in ein ROM
EMU = nes # Legt den Emulator fest, mit dem wir unser ROM testen
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
all: tools dist/hello.nes

# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/hello.nes
  
#deploy: all
#	$(DEPLOYER) dist/hello.nes

# erstellt ausführbare Hilfs-Programme
tools: tools/chr2asciiart tools/asciiart2chr

# erstellt ausführbares Hilfs-Programm 'chr2asciiart'
tools/chr2asciiart: tools/chr2asciiart.c
	$(CC) $(CFLAGS) $< -o $@

# erstellt ausführbares Hilfs-Programm 'asciiart2chr'
tools/asciiart2chr: tools/asciiart2chr.c
	$(CC) $(CFLAGS) $< -o $@

# erzeugt das eigentliche ROM
dist/hello.nes: bin/hello.prg bin/hello.chr
	cat $^ > $@
 
# erstellt den .text-Bereich des ROMs
bin/hello.prg: ./NES_ROM_LAYOUT.link obj/main.o
	$(LINKER) -o bin/hello.prg -C $^
 
# kompiliert die Quelldatei
obj/main.o: src/main.asm src/nes.inc
	cd src && $(ASSEMBLER) $(ASSFLAGS) main.asm -o ../obj/main.o && cd ..

# fügt beide Pattern Tables zusammen
bin/hello.chr: bin/pattern_table_1.chr bin/pattern_table_2.chr
	cat $^ > $@
  
# wandelt die ASCII-Tabelle in ein CHR-ROM um
bin/pattern_table_1.chr: res/pattern_table_1.txt
	tools/asciiart2chr $< $@

# wandelt die ASCII-Tabelle in ein CHR-ROM um
bin/pattern_table_2.chr: res/pattern_table_2.txt
	tools/asciiart2chr $< $@

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
