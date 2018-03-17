################################################################################
# TOOLS                                                                        #
################################################################################

You will need a copy of the following tools to build and run this code:

+ mipsgcc (psx) in your $PATH 

+ the following environment variables must be set to the corresponding 
directories of your toolchain (even when you use Windows use slashes ('/') to 
seperate different directories:
  * MIPS_TEMP
  * MIPS_COMPILER_PATH
  * MIPS_INCLUDE_PATH
  * MIPS_CPLUS_INCLUDE_PATH
  * MIPS_LIBRARY_PATH
  
+ some PSX emulator which main binary file is called 'psx' or 'psx.exe' in your 
  $PATH
  
Then you should call 'make' to build the code and 'make run' to test it within
the emulator.