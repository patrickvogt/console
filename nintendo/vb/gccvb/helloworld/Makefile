################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/hello.vb obj/main.o

CCVB = v810-gcc # Damit wird unser Quellcode in eine Objektdatei kompiliert
OBJCOPY = v810-objcopy # Das ROM in ein gültiges VB-ROM umwandeln
#DEPLOYER = # Legt das Programm fest, um das ROM auf eine Karte zu übertragen
EMU = vb  # Legt den Emulator fest, mit dem wir unser ROM testen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
CFLAGSVB = -Wall -nodefaultlibs -mv810 -xc # Argumente für den Cross-Compiler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run deploy

all: dist/hello.vb

dist/hello.vb: src/main.c
	 $(CCVB) $(CFLAGSVB) -I./include -o obj/main.o src/main.c
	 $(OBJCOPY) -S -O binary obj/main.o dist/hello.vb

run: dist/hello.vb
	$(EMU) dist/hello.vb
  
#deploy: dist/hello.vb
#	$(DEPLOYER) dist/hello.vb

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
