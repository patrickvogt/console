#ifndef __VIDEO_H
#define __VIDEO_H

#include "types.h"
#include "vip.h"


/***** Display RAM *****/
u32* const	L_FRAME0 =	(u32*)0x00000000;				// Left Frame Buffer 0
#define		CharSeg0		 0x00006000					// Characters 0-511
u32* const	L_FRAME1 =	(u32*)0x00008000;				// Left Frame Buffer 1
#define		CharSeg1		 0x0000E000					// Characters 512-1023
u32* const	R_FRAME0 =	(u32*)0x00010000;				// Right Frame Buffer 0
#define		CharSeg2		 0x00016000					// Characters 1024-1535
u32* const	R_FRAME1 =	(u32*)0x00018000;				// Right Frame Buffer 1
#define		CharSeg3		 0x0001E000					// Characters 1536-2047
#define		BGMMBase		 0x00020000					// Base address of BGMap Memory
u16* const	BGMM =		(u16*)BGMMBase;					// Pointer to BGMM
#define		BGMap(b)		 (BGMMBase + (b * 0x2000))	// Address of BGMap b (0 <= b <= 13)

#define		WAMBase			 0x0003D800					// Base address of World Attribute Memory
u16* const	WAM =		(u16*)WAMBase;					// Pointer to WAM
#define		World(w)		 (WAMBase + (w * 0x0020))	// Address of World w (0 <= w <= 31)
u16* const	CLMN_TBL =	(u16*)0x0003DC00;				// Base address of Column Tables
#define		OAMBase			 0x0003E000					// Base address of Object Attribute Memory
u16* const	OAM =		(u16*)OAMBase;					// Pointer to OAM
#define		Object(o)		 (OAMBase + (o * 0x0008))	// Address of Obj o (0 <= o <= 1023)

/* Macro to set the brightness registers */
#define	SET_BRIGHT(a,b,c)       VIP_REGS[BRTA]=(u16)(a); VIP_REGS[BRTB]=(u16)(b); VIP_REGS[BRTC]=(u16)(c)

/* Macro to set the GPLT (BGMap palette) */
#define	SET_GPLT(n,pal)         VIP_REGS[GPLT0+n]=pal

/* Macro to set the JPLT (OBJ palette) */
#define	SET_JPLT(n,pal)         VIP_REGS[JPLT0+n]=pal


/* Delay execution */
/*
void vbWaitFrame(u16 count) {
	u16 i;

	for (i = 0; i < count; i++) {
		VIP_REGS[INTCLR] = VIP_REGS[INTPND];
		while (!(VIP_REGS[INTPND] & XPEND)); //FRAMESTART
	}
}
*/
//*
void vbWaitFrame(u16 count) {
	u16 i;

	for (i = 0; i <= count; i++) {
		while (!(VIP_REGS[XPSTTS] & XPBSYR));
		while (VIP_REGS[XPSTTS] & XPBSYR);
	}
}


/* Turn the display on */
void vbDisplayOn() {
	VIP_REGS[REST] = 0;
	VIP_REGS[XPCTRL] = VIP_REGS[XPSTTS] | XPEN;
	VIP_REGS[DPCTRL] = VIP_REGS[DPSTTS] | (SYNCE | RE | DISP);
	VIP_REGS[FRMCYC] = 0;
	VIP_REGS[INTCLR] = VIP_REGS[INTPND];
	//while (!(VIP_REGS[DPSTTS] & 0x3C));  //required?

	VIP_REGS[BRTA]  = 0;
	VIP_REGS[BRTB]  = 0;
	VIP_REGS[BRTC]  = 0;
	VIP_REGS[GPLT0] = 0xE4;	/* Set all eight palettes to: 11100100 */
	VIP_REGS[GPLT1] = 0xE4;	/* (i.e. "Normal" dark to light progression.) */
	VIP_REGS[GPLT2] = 0xE4;
	VIP_REGS[GPLT3] = 0xE4;
	VIP_REGS[JPLT0] = 0xE4;
	VIP_REGS[JPLT1] = 0xE4;
	VIP_REGS[JPLT2] = 0xE4;
	VIP_REGS[JPLT3] = 0xE4;
	VIP_REGS[BKCOL] = 0;	/* Clear the screen to black before rendering */
}

/* Turn the display off */
void vbDisplayOff() {
	VIP_REGS[REST] = 0;
	VIP_REGS[XPCTRL] = 0;
	VIP_REGS[DPCTRL] = 0;
	VIP_REGS[FRMCYC] = 0;
	VIP_REGS[INTCLR] = VIP_REGS[INTPND];
}

/* Call this after the display is on and you want the image to show up */
void vbDisplayShow() {
	VIP_REGS[BRTA] = 32;
	VIP_REGS[BRTB] = 64;
	VIP_REGS[BRTC] = 32;
}

/* Call this to hide the image; e.g. while setting things up */
void vbDisplayHide() {
	VIP_REGS[BRTA] = 0;
	VIP_REGS[BRTB] = 0;
	VIP_REGS[BRTC] = 0;
}

void vbFXFadeIn(u16 wait) {
	u8 i;

	for (i = 0; i <= 32; i++) {
		vbWaitFrame(wait);
		SET_BRIGHT(i,i*2,i);
	}
}

void vbFXFadeOut(u16 wait) {
	s8 i;

	for (i = 32; i >= 0; i--) {
		vbWaitFrame(wait);
		SET_BRIGHT(i,i*2,i);
	}
}


u8 const colTable[128] = {
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xE0, 0xBC,
	0xA6, 0x96, 0x8A, 0x82, 0x7A, 0x74, 0x6E, 0x6A,
	0x66, 0x62, 0x60, 0x5C, 0x5A, 0x58, 0x56, 0x54,
	0x52, 0x50, 0x50, 0x4E, 0x4C, 0x4C, 0x4A, 0x4A,
	0x48, 0x48, 0x46, 0x46, 0x46, 0x44, 0x44, 0x44,
	0x42, 0x42, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40,
	0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3C,
	0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C,
	0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C
};

// Setup the default Column Table
void vbSetColTable() {
	u8 i;

	for (i = 0; i <= 127; i++) {
		CLMN_TBL[i      ] = colTable[i];
		CLMN_TBL[i + 256] = colTable[i];
		CLMN_TBL[i + 128] = colTable[127 - i];
		CLMN_TBL[i + 384] = colTable[127 - i];
	}
}


#endif
