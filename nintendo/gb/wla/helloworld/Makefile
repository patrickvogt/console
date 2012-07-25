################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.gb obj/main.o

ASSEMBLER = wla-gb # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = wlalink # Dieser 'linkt' unsere Objektdateien in ein ROM
#DEPLOYER = # Legt das Programm fest, um das ROM auf eine Karte zu übertragen
EMU = gb  # Legt den Emulator fest, mit dem wir unser ROM testen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
ASSFLAGS = # Argumente für den Assembler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run deploy clean

all: dist/hello.gb

dist/hello.gb: src/main.asm src/tiles.inc src/memory.asm src/gb.inc ./main.link
	 cd src && $(ASSEMBLER) -vo main.asm ../obj/main.o && cd ..
	 $(LINKER) -vr ./main.link dist/hello.gb

run: dist/hello.gb
	$(EMU) dist/hello.gb
  
#deploy: dist/hello.gb
#	$(DEPLOYER) dist/hello.gb

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
