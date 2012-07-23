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

		                                          /* Öffne TXT-Datei im Textmodus */
		in_file = fopen(argv[1], "rt");
		                                             /* Datei öffnen erfolgreich? */
		if(NULL == in_file)
		{
			                 /* NEIN => Benutzer informieren und Programm abbrechen */
		  (void) printf("Datei %s konnte nicht geöffnet werden\n", argv[1]);
		  return EXIT_FAILURE;
		}

		                        /* Öffne Ausgabedatei zum Schreiben im Binärmodus */
		out_file = fopen(argv[2], "wb");
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

		                        /* Bitmaske zum Speichern der Bits initialisieren */
		  unsigned int bit_mask = 0x80;                     /* 0x80 == %1000 0000 */

			                                    /* Temporärer Speicher für ein Byte */
			unsigned int tmp_byte = 0x0;

			                                                /* für jedes der 8 Bits */
		  for(i = 0; (i < 8) && (readi != EOF); i++)
		  { 
				if('#' == readi)
		    {
					                         /* (7-i). Bit mithilfe der Bitmaske setzen */
		      tmp_byte = tmp_byte | bit_mask;
		    }
		    else if('.' == readi)
		    {
					                                                      /* Nichts tun */
		    }
		    else
		    {
					                      /* Ein Zeichen sollte immer '.' oder '#' sein */
		      (void) printf("Zeichen ungleich '.' oder '#' gefunden: %c \n", readi);
		    }  

				              /* Bitmaske für nächsten Schleifendurchlauf vorbereiten */
		    bit_mask = bit_mask >> 1;

				                  /* Nächstes Zeichen einlesen und '\n's überspringen */
				do
				{
					readi = fgetc(in_file);
				}
				while(readi == '\n');
		  }

			                                             /* Byte in Datei schreiben */
			(void) fputc(tmp_byte, out_file);
		
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

