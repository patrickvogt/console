#ifndef __MEM_H
#define __MEM_H

#include "types.h"


u8* const	EXPANSION =	(u8*)0x04000000;				// Expansion bus area
u8* const	WORKRAM =	(u8*)0x05000000;				// Scratchpad RAM; USE WITH CAUTION!
														// (In fact, just leave it alone!)
u16* const	SAVERAM =	(u16*)0x06000000;				// Cartridge's Battery-backed SRAM

/***** Ancillary Functions *****/

// Copy a block of data from one area in memory to another.
void copymem (u8* dest, const u8* src, u16 num)
{
	u16 i;
	for (i = 0; i < num; i++) *dest++ = *src++;
}

// Set each byte in a block of data to a given value.
void setmem (u8* dest, u8 src, u16 num)
{
	u16 i;
	for (i = 0; i < num; i++) *dest++ = src;
}

/*	Copy a block of data from one area in memory to another,
adding a given value to each byte, first.	*/
void addmem (u8* dest, const u8* src, u16 num, u8 offset) {
	u16 i;
	for (i = 0; i < num; i++) *dest++ = (*src++ + offset);
}

#endif
