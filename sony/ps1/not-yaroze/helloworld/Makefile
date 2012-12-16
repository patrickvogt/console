################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = tools/exefixup dist/hello.exe bin/hello.psx obj/hello.o

INCLUDE = include
LIB = lib

COMPILER = mipsgcc # Damit wird unser Quellcode in eine Objektdatei kompiliert
LINKER = mipsgcc # Dieser 'linkt' unsere Objektdateien in ein ROM
FIXER = exefixup # Repariert bzw. validiert u.a. den Header von unserem ROM
EMU = psx # Legt den Emulator fest, mit dem wir unser ROM testen
#DEPLOYER = xlink # Legt das Programm fest, um das ROM auf eine Karte zu übertragen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
TFLAGS = # Argumente für den Ziel-Compiler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und müssen daher immer
# ausgeführt werden

.PHONY: all run clean tools deploy

# Standard-Target wird automatisch bei einem 'make' ausgeführt
all: tools dist/hello.exe

# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/hello.exe
  
#deploy: all
#	$(DEPLOYER) x dist/hello.exe

# erstellt ausführbare Hilfs-Programme
tools: tools/exefixup

# erzeugt das eigentliche ROM
dist/hello.exe: bin/hello.psx
	tools/exefixup $< $@
 
# erstellt den .text-Bereich des ROMs
bin/hello.psx: obj/hello.o obj/htsprint.o 
	$(LINKER) -v -L $(MIPS_LIBRARY_PATH) -L $(LIB) -Xlinker -mpsx -o $@ $^ -lps
 
# kompiliert die Quelldatei
obj/hello.o: src/hello.c include/htsprint.h
	$(COMPILER) -v -I $(MIPS_INCLUDE_PATH) -I $(MIPS_CPLUS_INCLUDE_PATH) -I $(INCLUDE) -c $< -o $@ -O2
	
tools/exefixup: tools/exefixup.c
	$(CC) $(CFLAGS) $< -o $@

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
