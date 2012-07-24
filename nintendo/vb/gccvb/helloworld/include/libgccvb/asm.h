#ifndef __ASM_H
#define __ASM_H

#define INT_ENABLE		asm("CLI;")
#define INT_DISABLE		asm("SEI;")

u32 jump_addr(void *addr);

    asm(".globl _jump_addr");
    asm("_jump_addr:");
    asm("add	-4,sp;");
    asm("st.w	lp,0[sp];");
    asm("jal	__dojump;");
    asm("ld.w	0[sp],lp;");
    asm("add	4,sp;");
    asm("jmp	[lp];");
    asm("__dojump:");
    asm("jmp	[r6];");


void inline set_intlevel(u8 level) {
	asm(
	    "stsr	sr5,r5;"
	    "movhi	0xFFF1,r0,r6;"
	    "movea	0xFFFF,r6,r6;"
	    "and		r5,r6;"
	    "mov		%0,r5;"
	    "andi	0x000F,r5,r5;"
	    "shl		0x10,r5;"
	    "or		r6,r5;"
	    "ldsr	r5,sr5;"
	: /* Output */
	: "r" (level) /* Input */
	: "r5", "r6" /* Clobber */
	);
}


int inline get_intlevel() {
	int level;

	asm(
	    "stsr	sr5,r5;"
	    "shr	0x10,r5;"
	    "andi	0x000F,r5,r5;"
	    "mov	r5,%0;"
	: "=r" (level) /* Output */
	: /* Input */
	: "r5" /* Clobber */
	);
	return level;
}

#endif
