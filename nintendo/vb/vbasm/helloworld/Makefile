################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.vb

ASSEMBLER = vbasm # Damit wird unser Assemblercode in eine Objektdatei kompiliert
#DEPLOYER = # Legt das Programm fest, um das ROM auf eine Karte zu übertragen
EMU = vb  # Legt den Emulator fest, mit dem wir unser ROM testen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
ASSFLAGS = # Argumente für den Assembler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run deploy

all: dist/hello.vb

dist/hello.vb: src/hello.asm
	 cd src && $(ASSEMBLER) hello.asm -o ../dist/hello.vb && cd ..
 
run: dist/hello.vb
	$(EMU) dist/hello.vb
  
#deploy: dist/hello.vb
#	$(DEPLOYER) dist/hello.vb

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
