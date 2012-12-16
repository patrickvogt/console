/* 
 * exefixup.c v0.02 Andrew Kieschnick <andrewk@mail.utexas.edu>
 *
 * displays PS-X EXE header information
 * offers to fix incorrect t_size
 * offers to pad to 2048-byte boundary for cd-rom use
 *
 * THIS SOURCE WAS MODIFIED (SLIGHTLY) TO WORK UNDER DOS
 * IF YOU USE UNIX, GET THE THE UNMODIFIED SOURCE
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/* CHANGES by Patrick Vogt (2012-12-16):                 
 * -Modified to become compatible with the latest gcc 
 * -Added an outfilename for the fixed/padded file
 * -Remove question to always fix/pad
 * -commented a bit ;-) 
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* converts four chars into one int */
unsigned int char2int(unsigned char *foo)
{
  return foo[3]*16777216 + foo[2]*65536 + foo[1]*256 + foo[0];
}

/* converts one given int into four chars */
void int2char(unsigned int foo, unsigned char *bar)
{
  bar[3]=foo>>24;
  bar[2]=foo>>16;
  bar[1]=foo>>8;
  bar[0]=foo;
}

/* prints the usage of the exefixup tool */
void usage(void)
{
  (void) printf("Usage: exefixup <infilename> <outfilename>\n\n");
  (void) printf("\t<infilename>\ta PS-X EXE file\n\n");
  (void) printf("\tdisplays EXE header\n");
  (void) printf("\toffers to correct a wrong t_size\n");
  (void) printf("\toffers to pad to 2048-byte boundary\n\n");
  
  exit(0);
}

int main(int argc, char *argv[])
{
  FILE *exe;
  FILE *out;
  unsigned char data[8];
  char infilename[256];
  char outfilename[256];
  int i;
  unsigned int header_data[12];
  unsigned int size;
  unsigned int padsize;

  /* usage correct? */
  if(argc!=3)
  {
    usage();
  }
  
  (void) strncpy(infilename, argv[1], 256);
  (void) strncpy(outfilename, argv[2], 256);

  /* open psx file */
  exe=fopen(infilename, "rb");
  
  (void) printf("\n\n");

  if(NULL==exe)
  {
    /* could not open file */
    (void) printf("ERROR: Can't open %s\n",infilename);
    exit(-1);
  }

  for(i=0; i<8; i=i+1)
  {
    (void) fscanf(exe, "%c", &data[i]);
  }
  data[8]='\0';

  /* valid psx exe file? */
  if(0!=strncmp((const char *)data, "PS-X EXE", 8))
  {
    /* not a valid psx file! */
    (void) printf("ERROR: Not a PS-X EXE file\n");
    exit(-1);
  }
  
  /* read header */
  for(i=0; i<12; i=i+1)
  {
    (void) fscanf(exe, "%c", &data[0]);
    (void) fscanf(exe, "%c", &data[1]);
    (void) fscanf(exe, "%c", &data[2]);
    (void) fscanf(exe, "%c", &data[3]);
    header_data[i]=char2int(data);
  }

  /* print header */
  (void) printf("id\tPS-X EXE\n");
  (void) printf("text\t0x%.8x\n", header_data[0]);
  (void) printf("data\t0x%.8x\n", header_data[1]);
  (void) printf("pc0\t0x%.8x\n", header_data[2]);
  (void) printf("gp0\t0x%.8x\n", header_data[3]);
  (void) printf("t_addr\t0x%.8x\n", header_data[4]);
  (void) printf("t_size\t0x%.8x\n", header_data[5]);
  (void) printf("d_addr\t0x%.8x\n", header_data[6]);
  (void) printf("d_size\t0x%.8x\n", header_data[7]);
  (void) printf("b_addr\t0x%.8x\n", header_data[8]);
  (void) printf("b_size\t0x%.8x\n", header_data[9]);
  (void) printf("s_addr\t0x%.8x\n", header_data[10]);
  (void) printf("s_size\t0x%.8x\n\n", header_data[11]);

  /* go to the end of the psx exe file */
  (void) fseek(exe, 0, SEEK_END);
  
  size=ftell(exe)-2048;

  /* check if filesoze is a multiple of 2048 */
  padsize=2048-(size%2048);
  if(padsize!=2048)
  {
    /* filesize is not a multiple of 2048 */
    (void) printf("WARNING: EXE size is not a multiple of 2048! I'll pad that for you.\n");
    
    out = fopen(outfilename, "wb");
    
    header_data[5]=size+padsize;

    (void) fprintf(out, "PS-X EXE");
    
    for(i=0; i<12; i=i+1)
    {
      int2char(header_data[i], data);
      (void) fprintf(out, "%c%c%c%c", data[0], data[1], data[2], data[3]);
    }

    (void) fseek(exe, 56, SEEK_SET);

    for(i=0; i<size+1992; i=i+1)
    {
      (void) fscanf(exe, "%c", &data[0]);
      (void) fprintf(out, "%c", data[0]);
    }
    
    /* fill the rest of the file with zeros */
    for(i=0; i<padsize; i++)
    {
      (void) fprintf(out, "%c", 0);
    }
    
    size=header_data[5];
    (void) fclose(out);
  }
  
  /* is size in header correct? */
  if(size!=header_data[5])
  {
    (void) printf("WARNING: EXE header t_size does not match filesize-2048\n");
    (void) printf("EXE header:\t 0x%.8x bytes\n", header_data[5]);
    (void) printf("filesize-2048:\t 0x%.8x bytes\n", size);

    out = fopen(outfilename, "wb");

    (void) fprintf(out, "PS-X EXE");
    
    /*write header till size */
    for(i=0; i<5; i=i+1)
    {
      int2char(header_data[i], data);
      (void) fprintf(out, "%c%c%c%c", data[0], data[1], data[2], data[3]);
    }
  
    /* write correct size */
    int2char(size, data);
    (void) fprintf(out, "%c%c%c%c", data[0], data[1], data[2], data[3]);
    
    /* write header after size */
    for(i=6; i<12; i=i+1)
    {
      int2char(header_data[i], data);
      (void) fprintf(out, "%c%c%c%c", data[0], data[1], data[2], data[3]);
    }
  
    /* jump to real data */
    (void) fseek(exe, 56, SEEK_SET);

    /* copy the rest of the file */
    for(i=0; i<size+1992; i=i+1)
    {
      (void) fscanf(exe, "%c", &data[0]);
      (void) fprintf(out, "%c", data[0]);
    }
    
    (void) fclose(out);

  }
  
  (void) fclose(exe);
  
  return EXIT_SUCCESS;
}
