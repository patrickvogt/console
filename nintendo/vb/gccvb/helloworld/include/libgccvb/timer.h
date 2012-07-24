#ifndef _TIMER_H_
#define _TIMER_H_

#include "types.h"
#include "hw.h"


//use with 20us timer (range = 0 to 1300)
#define TIME_US(n)		(((n)/20)-1)

//use with 100us timer (range = 0 to 6500, and 0 to 6.5)
#define TIME_MS(n)		(((n)*10)-1)
#define TIME_SEC(n)		(((n)*10000)-1)


#define TIMER_ENB		0x01
#define TIMER_ZSTAT		0x02
#define TIMER_ZCLR		0x04
#define TIMER_INT		0x08
#define TIMER_20US		0x10
#define TIMER_100US		0x00


void timer_enable(int enb);
u16 timer_get();
void timer_set(u16 time);
void timer_freq(int freq);
void timer_int(int enb);
int timer_getstat();
void timer_clearstat();

u8 tcr_val = 0;


void timer_enable(int enb) {
	if (enb) tcr_val |= TIMER_ENB;
	else tcr_val &= ~TIMER_ENB;
	HW_REGS[TCR] = tcr_val;
}

u16 timer_get() {
	return (HW_REGS[TLR] | (HW_REGS[THR] << 8));
}

void timer_set(u16 time) {
	HW_REGS[TLR] = (time & 0xFF);
	HW_REGS[THR] = (time >> 8);
}

void timer_freq(int freq) {
	if (freq) tcr_val |= TIMER_20US;
	else tcr_val &= ~TIMER_20US;
	HW_REGS[TCR] = tcr_val;
}

void timer_int(int enb) {
	if (enb) tcr_val |= TIMER_INT;
	else tcr_val &= ~TIMER_INT;
	HW_REGS[TCR] = tcr_val;
}

int timer_getstat() {
	return (!!(HW_REGS[TCR] & TIMER_ZSTAT));
}

void timer_clearstat() {
	HW_REGS[TCR] = (tcr_val|TIMER_ZCLR);
}


#endif //_TIMER_H_
