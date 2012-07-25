################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.gb obj/hello.o

ASSEMBLER = RGBASM95 # Damit wird unser Assemblercode in eine Objektdatei kompiliert
LINKER = XLINK95 # Dieser 'linkt' unsere Objektdateien in ein ROM
FIXER = RGBFIX95 # Repariert bzw. validiert u.a. den Header von unserem ROM
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

.PHONY: all run deploy

all: dist/hello.gb

dist/hello.gb: src/hello.asm src/ibmpc1.inc src/memory.asm src/gbhw.inc ./hello.link
	 cd src && $(ASSEMBLER) -o../obj/hello.o hello.asm && cd ..
	 $(LINKER) -ts -mmemory.map ./hello.link
	 $(FIXER) -v dist/hello.gb

run: dist/hello.gb
	$(EMU) dist/hello.gb
  
#deploy: dist/hello.gb
#	$(DEPLOYER) dist/hello.gb

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
