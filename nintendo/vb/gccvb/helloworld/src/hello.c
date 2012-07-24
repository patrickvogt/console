//Include library, charmap, charset
#include "libgccvb/libgccvb.h"
#include "chadjustment.h"
#include "bgadjustment.h"

/** A good beginner's demo.  All this does is display the
 *  splashscreen for the PVB 2010 compo.  This demonstrates
 *  loading a charset and a bgmap into memory, then showing
 *  them in a world onscreen.
 */

int main()
{
    //Initial setup
    vbDisplayOn();

    //Copy tileset and tilemap into memory
    copymem((void*)CharSeg0, (void*)CHADJUSTMENT, 256*16);
    copymem((void*)BGMap(0), (void*)BGADJUSTMENT, 450*16);

    //Setup worlds
    //(This uses structs to access world data, the old method using functions is commented out)
    WA[31].head = (WRLD_LON|WRLD_OVR);
    WA[31].w = 384;
    WA[31].h = 224;
    //vbSetWorld(31, (WRLD_LON|WRLD_OVR), 0, 0, 0, 0, 0, 0, 384, 224);
    
    WA[30].head = (WRLD_RON|WRLD_OVR);
    WA[30].my = 224;
    WA[30].w = 384;
    WA[30].h = 224;
    //vbSetWorld(30, (WRLD_RON|WRLD_OVR), 0, 0, 0, 0, 0, 224, 384, 224);

    WA[29].head = WRLD_END;
    //vbSetWorld(29, WRLD_END, 0, 0, 0, 0, 0, 0, 0, 0);

    //Set brightness registers
    vbDisplayShow();

    //Main loop (Empty because we're done but don't want to reach the end)
    for (;;);

    return 0;
}
