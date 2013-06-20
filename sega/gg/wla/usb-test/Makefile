################################################################################
# VARIABLEN                                                                    #
################################################################################

# Dateien, die durch ein 'clean' gelöscht werden sollen
CLEANABLE_OBJS = dist/usb.gg obj/fifo.o

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
all: dist/usb.gg

dist/usb.gg: src/fifo.asm ./fifo.link
	 cd src && $(ASSEMBLER) -vo fifo.asm ../obj/fifo.o && cd ..
	 $(LINKER) -rv ./fifo.link dist/usb.gg
  
# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/usb.gg
  
#deploy: all
#	$(DEPLOYER) dist/usb.gg

# löscht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
