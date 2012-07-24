#ifndef __MISC_H
#define __MISC_H

#include "types.h"
#include "mem.h"
#include "video.h"


#define tabsize 4 //horizontal tab size in chars

const char nums[16]="0123456789ABCDEF";

void cls() {
	setmem((void*)(BGMap(0)), 0, 8192);
}

void vbTextOut(u16 bgmap, u16 col, u16 row, char *t_string)
/* The font must reside in Character segment 3 */
{
	u16 i = 0,
		pos = row * 64 + col;

	while(t_string[i])
	{
		BGMM[(0x1000 * bgmap) + pos + i] = (u16)t_string[i] - 32 + 0x600;
		i++;
	}
}


void vbPrint(u8 bgmap, u16 x, u16 y, char *t_string, u16 bplt)
{
// Font consists of the last 256 chars (1792-2047)
	u16 i=0,pos=0,col=x;

	while(t_string[i])
	{
		pos = (y << 6) + x;

		switch(t_string[i])
		{
			case 7:
				// Bell (!)
				break;
			case 9:
				// Horizontal Tab
				x = (x / tabsize + 1) * tabsize;
				break;
			case 10:
				// Carriage Return
				y++;
				x = col;
				break;
			case 13:
				// Line Feed
				// x = col;
				break;
			default:
				BGMM[(0x1000 * bgmap) + pos] = ((u16)t_string[i] + 0x700) | (bplt << 14);
				if (x++ > 63)
				{
					x = col;
					y++;
				}
				break;
		}
		i++;
	}
}


#endif
