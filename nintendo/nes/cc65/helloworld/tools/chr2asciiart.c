/******************************************************************************/
/* 'HEADER'-ABSCHNITT                                                         */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]);

/******************************************************************************/
/* IMPLEMENTATION-ABSCHNITT                                                   */
/******************************************************************************/

int main(int argc, char *argv[])
{
	                                      /* Programm benötigt 2 Startargumente */
	if(3 == argc)
	{
		                                           /* Zeiger auf zu lesende Datei */
		FILE *in_file = NULL;
		                                       /* Zeiger auf zu schreibende Datei */
		FILE *out_file = NULL;
		                              /* temporäre Variable zum Lesen eines Bytes */
		unsigned int readi=0;
		                     /* Zähler, um die Anzahl der einzelnen Bits zu lesen */
		unsigned int bit_counter=0;		

		    /* Öffne CHR-Datei im Binärmodus (abschalten von Textkonvertierungen) */
		in_file = fopen(argv[1], "rb");
		                                             /* Datei öffnen erfolgreich? */
		if(NULL == in_file)
		{
			                 /* NEIN => Benutzer informieren und Programm abbrechen */
		  (void) printf("Datei %s konnte nicht geöffnet werden\n", argv[1]);
		  return EXIT_FAILURE;
		}

		                         /* Öffne Ausgabedatei zum Schreiben im Textmodus */
		out_file = fopen(argv[2], "wt");
		                                             /* Datei öffnen erfolgreich? */
		if(NULL == out_file)
		{
			                 /* NEIN => Benutzer informieren und Programm abbrechen */
		  (void) printf("Datei %s konnte nicht zum Schreiben angelegt werden\n", 
				argv[2]);		  
			return EXIT_FAILURE;
		}

		                                               /* Erstes Zeichen einlesen */
		readi = fgetc(in_file);
		
		                    /* Solange bis wir nicht das Dateiende erreicht haben */
		while(EOF != readi)
		{
			                                     /* interne Zähl-/Schleifenvariable */
		  int i = 0;

		                      /* Bitmaske zum Analysieren der Bits initialisieren */
		  unsigned int bit_mask = 0x80;                     /* 0x80 == %1000 0000 */

			                                                /* für jedes der 8 Bits */
		  for(i = 0; i < 8; i++)
		  { 
				                   /* Extrahiere das (7-i). Bit mithilfe der Bitmaske */
		    unsigned int bit = readi & bit_mask; 
				                 /* Schiebe das extrahierte Bit nun auf die 0. Stelle */
		    bit = bit >> (7-i);

				                   /* Zähle die Bits intern zur späteren Formatierung */
		    bit_counter++;

				                                                         /* Teste Bit */
		    if(0 == bit)
		    {
					      /* Es ist eine 0, die durch ein '.' repräsentiert werden soll */
		      (void) fputc('.', out_file);
		    }
		    else if(1 == bit)
		    {
					      /* Es ist eine 1, die durch ein '#' repräsentiert werden soll */
		      (void) fputc('#', out_file);
		    }
		    else
		    {
					                              /* Ein Bit sollte immer 0 oder 1 sein */
		      (void) printf("Bit ungleich 0 oder 1 gefunden: %x \n", bit);
		    }     

				              /* Bitmaske für nächsten Schleifendurchlauf vorbereiten */
		    bit_mask = bit_mask >> 1;
		  }

		                               /* Neue Zeile beginnen nach jeweils 8 Bits */
		  (void) fputc('\n', out_file);

		                            /* Eine Zeile Abstand nach 8 Bytes == 64 Bits */
		  if(0 == bit_counter % 64)
		  {
		    (void) fputc('\n', out_file);
		  }

			                              /* Lese das nächste Zeichen aus der Datei */
		  readi = fgetc(in_file);		
		}

		                                       /* Schließe die geöffneten Dateien */
		fclose(in_file);
		fclose(out_file);
	}
	else
	{
			   /* Zeige dem Benutzer, welche Startargumente übergeben werden müssen */
		(void) printf("USAGE: %s INFILE OUTFILE\n", argv[0]);
	}  

  return EXIT_SUCCESS;  
}

