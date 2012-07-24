################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.smc obj/main.o

ASSEMBLER = wla-65816 # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = wlalink # Dieser 'linkt' unsere Objektdateien in ein ROM
EMU = snes  # Legt den Emulator fest, mit dem wir unser ROM testen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
ASSFLAGS = # Argumente für den Assembler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run clean deploy

dist/hello.smc: src/header.inc src/main.asm src/init.asm \
    src/load_graphics.asm src/snes.inc src/tiles.inc ./hello.link
	 cd src && $(ASSEMBLER) -vo main.asm ../obj/main.o && cd ..
	 $(LINKER) -vr ./hello.link dist/hello.smc

run: dist/hello.smc
	$(EMU) dist/hello.smc

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	rm $(CLEANABLE_OBJS)

