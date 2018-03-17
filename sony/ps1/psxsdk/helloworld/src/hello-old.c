#include <syscall.h>
#include <psxtypes.h>
#include <int.h>
#include <gpu.h>
#include "htsprint.h"

#define OT_LENGTH 5 /* Laenge der Ordering Table */

GsOT_TAG zSortTable[1<<OT_LENGTH];
PACKET cmdBuffer[3000];

int main(int argc, char *argv[])
{  
  COLOR clr_color = {0,0,0,0}; /* Hintergrundfarbe schwarz */

  PsxUInt32 outputBufferIndex; /* aktueller aktiver Buffer */
  GsOT worldOrderTable; /* Ordering Table */
  
  INT_SetUpHandler(); /* InterruptHandler initialisieren */

  /* hier folgende die Grafikprozssorbefehle zur Darstellung des Textes:
  
    -GPU_SetPal(1) sorgt fuer die Auswahl der Fernsehnorm (NTSC/PAL)
      hier ist dies PAL
      
    -GPU_Reset(0) fuehrt einen Reset des Grafikprozessors aus
    
    -GPU_Init(320,240,GsNONINTER,0) definiert den Screenmode auf dem 
      der Text dargestellt werden soll. Hier: 320x240 Pixel
      
    -GPU_DefDispBuffer(0,0,0,0) - Initialisierung des Double Bufferings
  
  */
  
  GPU_SetPAL(MODE_PAL); /* PAL */
  GPU_Reset(0);  /* GPU zuruecksetzen */                         
  GPU_Init(320, 240, GsNONINTER, 0); /* ... und initialisieren (320x240, non-interlaced) */    
  GPU_DefDispBuff(0, 0, 0, 240); /* Double-Buffer initialisieren */

  worldOrderTable.length = OT_LENGTH; /* Definition der OrderingTable */
  worldOrderTable.org = zSortTable;

  GPU_EnableDisplay(1); /* Bildschirm einschalten */

  SetupFont(); /* Zeichensatz laden */

  for(;;)    /* Start der Endlosschleife */
  {
    /* Initialisierung der Ordering Table und des Doublebuffering Speichers */
    outputBufferIndex = GPU_GetActiveBuff();
    GPU_ClearOt(0, 0, &worldOrderTable);                
    GPU_SetCmdWorkSpace(cmdBuffer);   /* setzt den Befehlsspeicher */                           
    GPU_SortClear(0, &worldOrderTable, &clr_color); /* löscht Bildschirm und setzt Hindergrundfarbe */

    Print(&worldOrderTable,10,24,"Hallo Welt");  /* Ausgabe des Textes */

    GPU_VSync(); /* warte auf Vertical-Interrupt */
    GPU_DrawSync(); /* warte bis fertig gezeichnet */
    GPU_DrawOt(&worldOrderTable); /* zeichne OT */
    
  }   /* Ende der Endlosschleife */
  
  INT_ResetHandler();
  
  return 0;
}
