#include <psx.h>
#include <stdio.h>

unsigned int prim_list[0x4000];

volatile int display_is_old = 1;
volatile int time_counter = 0;
int  dbuf=0;

void prog_vblank_handler() {
	display_is_old = 1;
	time_counter++;
}

int main() {
	PSX_Init();
	GsInit();
	GsSetList(prim_list);
	GsClearMem();
	GsSetVideoMode(320, 240, VMODE_PAL);
	GsLoadFont(768, 0, 768, 256);
	SetVBlankHandler(prog_vblank_handler);

	while(1) {
		if(display_is_old)	{
			dbuf=!dbuf;
			GsSetDispEnvSimple(0, dbuf ? 0 : 256);
			GsSetDrawEnvSimple(0, dbuf ? 256 : 0, 320, 240);

			GsSortCls(0,0,0);

			GsPrintFont(70, 120, "Hello World");
	
			GsDrawList();
			while(GsIsDrawing());

			display_is_old=0;
		}
	}

	return 0;
}
