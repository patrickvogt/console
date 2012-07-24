#ifndef __AUDIO_H
#define __AUDIO_H

#include "types.h"

typedef struct SOUNDREG
{
	//this table is for the most part untested, but looks to be accurate
	//                 |      D7      ||      D6      ||      D5      ||      D4      ||      D3      ||      D2      ||      D1      ||      D0      |
	u8 SxINT; //       [----Enable----][--XXXXXXXXXX--][-Interval/??--][--------------------------------Interval Data---------------------------------]
	u8 spacer1[3];
	u8 SxLRV; //       [---------------------------L Level----------------------------][---------------------------R Level----------------------------]
	u8 spacer2[3];
	u8 SxFQL; //       [------------------------------------------------------Frequency Low Byte------------------------------------------------------]
	u8 spacer3[3];
	u8 SxFQH; //       [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--------------Frequency High Byte-------------]
	u8 spacer4[3];
	u8 SxEV0; //       [---------------------Initial Envelope Value-------------------][------U/D-----][-----------------Envelope Step----------------]
	u8 spacer5[3];
		 //Ch. 1-4 [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	         //Ch. 5   [--XXXXXXXXXX--][------E/D-----][----?/Short---][--Mod./Sweep--][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	u8 SxEV1; //Ch. 6  [--XXXXXXXXXX--][----------------------E/D---------------------][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	u8 spacer6[3];
	//Ch. 1-5 only (I believe address is only 3 bits, but may be 4, needs testing)
	u8 SxRAM; //       [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--------------Waveform RAM Address------------]
	u8 spacer7[3];
	//Ch. 5 only
	u8 S5SWP; //       [------CLK-----][-------------Sweep/Modulation Time------------][------U/D-----][----------------Number of Shifts--------------]
	u8 spacer8[35];
} SOUNDREG;


u8* const WAVEDATA1 =		(u8*)0x01000000;
u8* const WAVEDATA2 =		(u8*)0x01000080;
u8* const WAVEDATA3 =		(u8*)0x01000100;
u8* const WAVEDATA4 =		(u8*)0x01000180;
u8* const WAVEDATA5 =		(u8*)0x01000200;
u8* const MODDATA =			(u8*)0x01000280;
SOUNDREG* const SND_REGS =	(SOUNDREG*)0x01000400; //(SOUNDREG*)0x010003C0;
#define SSTOP				*(u8*)0x01000580

/***** Sound Register Mnemonics *****/
#define	WAVE1	0x00	// Voluntary wave channel #1
#define	WAVE2	0x01	// Voluntary wave channel #2
#define	WAVE3	0x02	// Voluntary wave channel #3
#define	WAVE4	0x03	// Voluntary wave channel #4
#define	SWEEP	0x04	// Sweep/modulation channel
#define	NOISE	0x05	// Pseudorandom noise channel

#endif
