################################################################################
# VARIABLEN                                                                    #
################################################################################
# TOOLCHAIN_PREFIX = C:\psx-sdk

# EXAMPLE_VMODE = VMODE_PAL

# CDLIC_FILE = $(TOOLCHAIN_PREFIX)/share/licenses/infoeur.dat

# CC = 
# LIB = -L$(TOOLCHAIN_PREFIX)/lib
# INCLUDE = -I$(TOOLCHAIN_PREFIX)/include
# LDSCRIPT = -T $(TOOLCHAIN_PREFIX)/mipsel-unknown-elf/lib/ldscripts/playstation.x
# CCFLAGS = -fsigned-char -msoft-float -mno-gpopt -fno-builtin -G0 $(LIB) $(INCLUDE) $(LDSCRIPT) 
	# mkdir -p cd_root
	# $(CC) $(CCFLAGS) testapp.c -o testapp.elf
	# elf2exe testapp.elf testapp.exe
	# cp testapp.exe cd_root
	# systemcnf testapp.exe > cd_root/system.cnf
	# mkisofs.exe -o testapp.hsf -V TESTAPP -sysid PLAYSTATION cd_root
	# mkpsxiso testapp.hsf testapp.bin $(CDLIC_FILE)

# Dateien, die durch ein 'clean' gel�scht werden sollen
CLEANABLE_OBJS = tools/exefixup dist/hello.exe bin/hello.psx obj/hello.o

INCLUDE = include
LIB = lib

COMPILER = mipsel-unknown-elf-gcc # Damit wird unser Quellcode in eine Objektdatei kompiliert
LINKER = mipsel-unknown-elf-gcc # Dieser 'linkt' unsere Objektdateien in ein ROM
FIXER = exefixup # Repariert bzw. validiert u.a. den Header von unserem ROM
EMU = psx # Legt den Emulator fest, mit dem wir unser ROM testen
#DEPLOYER = xlink # Legt das Programm fest, um das ROM auf eine Karte zu �bertragen
CC = gcc # C-Compiler
CFLAGS = -Wall -ansi -pedantic -g -O0 # C-Compiler-Argumente
TFLAGS = # Argumente f�r den Ziel-Compiler

################################################################################
# RULES                                                                        #
################################################################################

# 'Phony targets' sind Aktionen und keine Dateien und m�ssen daher immer
# ausgef�hrt werden

.PHONY: all run clean tools deploy

# Standard-Target wird automatisch bei einem 'make' ausgef�hrt
all: tools dist/hello.exe

# startet das erstellte ROM im Emulator
run: all
	$(EMU) dist/hello.exe
  
#deploy: all
#	$(DEPLOYER) x dist/hello.exe

# erzeugt das eigentliche ROM
dist/hello.exe: bin/hello.psx
	tools/exefixup $< $@
 
# erstellt den .text-Bereich des ROMs
bin/hello.psx: obj/hello.o obj/htsprint.o 
	$(LINKER) -v -L $(MIPS_LIBRARY_PATH) -L $(LIB) -Xlinker -mpsx -o $@ $^ -lps
 
# kompiliert die Quelldatei
obj/hello.o: src/hello.c include/htsprint.h
	$(COMPILER) -v -I $(MIPS_INCLUDE_PATH) -I $(MIPS_CPLUS_INCLUDE_PATH) -I $(INCLUDE) -c $< -o $@ -O2
	

# l�scht alle (durch dieses Makefile) erzeugten Dateien
clean:
	-rm $(CLEANABLE_OBJS)
