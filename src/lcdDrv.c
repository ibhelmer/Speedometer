/*----------------------------------------------------------------------------*/
/* Filnavn : lcddrv.c                                                         */
/* Dato    : 05.05.2018													      */
/* Version : 2.1            Initialer : IHN                                   */
/* Status  : Release        Placering : w:\src\srcPIC\release\LCDdriverV2.1   */
/* Beskrivelse :															  */
/*				LCD driver rutine. Anvender Microchip Harmony                 */
/*              Driver rutinerne kan anvendes sammen med standart LCD         */                    
/*		      	moduler der anvender LCD kontrolleren HD44780 eller      	  */
/*				kombatible. Denne koder er lavet til 8 bits mode med    	  */
/*				følgende forbindelse af kontrolsignaler :					  */
/*				PORTE bit 0-7 - DB  Data bus 								  */
/*              PORTB bit 15  - RS  register select							  */
/* 				PORTD bit 5   - RW  Read/Write								  */
/*				PORTE bit 4   - E   Enable bit                    		      */	
/*																			  */
/* 				Forbindes LCD modul anderledes skal LCD, E, RW og RS ændres.  */
/*				Dette kan gøres via Harmony Configurator                      */
/* 				Kald først lcdinit, herefter kan rutinerne for output         */
/*				anvendes (lcdprintf())                                        */
/*																		      */
/*              Følgende lcd rutiner findes i denne driver :                  */
/*				lcdPrintf(*) - Udskriver på lcd, virker som printf            */
/*              lcdClear()   - Sletter lcd 								      */
/*              lcdGoto(char pos) - Flytter cursor til pos                    */
/*                                                         Ib Helmer Nielsen  */
/*----------------------------------------------------------------------------*/
/*                  	 I T  - T E K N O L O G                               */
/*        U N I V E R S I T Y   C O L L E G E   N O R D J Y L L A N D S       */
/*----------------------------------------------------------------------------*/
/* Historik																      */
/* Version 1.0   : Lavet til 4 bits interface, kontrol og databus på samme    */
/*                 port kontrol signalerne RS, E og RW blev brugt.			  */
/*																			  */
/*	Version 1.1  : Lavet til 8 bits interface, RW fjernet (skal fast fortrådes*/
/*                 til GND), strobe ændret fra at være makro til at være en   */
/*                 funktion													  */
/* Version 1.2   : Rutinerne lcd_prstring og lcd_printf tilføjer              */
/* Version 1.3   : Porte til controlsignaler ændret                           */
/* Version 1.3.5 : Dokumentation revideret, define flyttet til header fil     */
/* Version 2.0   : Driver opdateret til pic18F4550                            */
/* Version 2.1   : Driver opdateret til PIC32MX975F512L, RW forbundet til port*/
/*----------------------------------------------------------------------------*/

#include "lcdDrv.h"			// Header fil til denne driver

/******************************************************************************/
/* Navn       : lcdInit	    											  	  */
/* Prototype  : void lcdinit(char mode)		    							  */
/* Programmør : IHN             Dato : 05.05.2018		Version : 2.1  		  */
/* Funktion   : Initialiserer LCD og opsætter LCD med cursor uden cursor eller*/
/* 				med eller uden blinkende cursor 				  			  */
/* Input      : char mode bestemmer opsætning af cursor   	  				  */
/*              mode = NO           - Ingen cursor ingen blinkengde blok      */
/*              mode = BLINK        - Blinkende blok                          */
/*              mode = CURSOR       - Cursor men ingen blinkende blok         */
/*              mode = CURSORBLINK  - Cursor og blinkende blok                */
/*  Output	  :															  	  */
/*					 -														  */
/*	Kalder	  :																  */
/*			    lcdcmd,DelayMs, DelayUs							              */
/******************************************************************************/
void lcdInit(unsigned char mode)
{
#if KONTRAST
    KONTRASTDir = OUTPUT;
    KONTRAST = 0;
#endif	
	DelayMs(15);
	lcdWriteInit(INIT);
	DelayMs(5);
	lcdWriteInit(INIT);
	DelayMs(2);
	lcdWriteInit(INIT);
    DelayMs(2);
	lcdCmd(SETUP);				 
	lcdCmd(DISPON+mode);
	lcdCmd(MOVE);
	lcdClear();
}

/******************************************************************************/
/* Navn       : lcdRead        					   				        	  */
/* Prototype  : unsigned char lcdwrite()                                      */
/* Programmør : ihn            Dato :	05.05.2018    Version : 2.1			  */
/* Funktion   : Læser data fra LCD controller								  */
/* 				 			                							  	  */
/* Input      : 															  */
/*                                                                        	  */
/* Output	  :															  	  */
/* 				unsigned char indeholder data fra lcd controller     		  */
/* Kalder	  :																  */
/*			    DelayUs()                  								      */
/******************************************************************************/
unsigned char lcdRead()
{
	unsigned char c;
	RSOff(); RWOn();
	LCD_DIR = 0xFF;
	EOn();
	DelayNs(500);
	c = LCD;
	EOff();
	return c;
}		  
/******************************************************************************/
/* Navn       : lcdWrite        									  		  */
/* Prototype  : void lcdWrite(unsigned char c)                                */
/* Programmør : ihn            Dato :	05.05.2018    Version : 2.1			  */
/* Funktion   : Sender data til LCD controller, venter til display er klar 	  */
/* 				før rutine udføres     							  		      */
/* Input      : 															  */
/* 				unsigned char c indeholder data til lcd controller     	      */
/*                                                                        	  */
/* Output	  :															  	  */
/*					 -														  */
/* Kalder	  :																  */
/*					                           								  */
/******************************************************************************/
void lcdWrite(unsigned char c)
{
    do {} while(lcdRead()& BUSY); 
    RSOn(); RWOff();
	LCD_DIR = 0x00;
	EOn();
	LCD = c;
	DelayNs(500);
	EOff();
    DelayUs(60);
    
}

/******************************************************************************/
/* Navn       : lcdCmd        					    				  		  */
/* Prototype  : void lcdCmd(unsigned char c)                                  */
/* Programmør : ihn            Dato :	05.05.2018    Version : 2.1			  */
/* Funktion   : Sender kommando til LCD controller, returner fra funktion  	  */
/* 				checke busy flag eller laver delay					  		  */
/* Input      : 															  */
/* 				unsigned char c indeholder kommando til lcd controller        */
/*                              			                                  */
/* Output	  :															  	  */
/*					 -														  */
/* Kalder	  :																  */
/*				DelayUs()					    						      */
/******************************************************************************/
void lcdCmd(unsigned char c)
{
    // Det forventes at display er klar!!!
	RSOff(); RWOff();
	LCD_DIR = 0x00;
	EOn();
	LCD = c;
	DelayNs(500);
	EOff();
#ifdef CHECKBUSY
    do {} while(lcdRead()& BUSY);
#else
    if ((c==0x01)||(c==0x02))
    {
        DelayMs(3);
    }
    else
    {
        DelayUs(60);
    } 
#endif	
}
/******************************************************************************/
/* Navn       : lcdWriteInit(unsigned char c)         				  		  */
/* Prototype  : void lcdWriteInit(unsigned char c)                            */
/* Programmør : ihn            Dato :	05.05.2018    Version : 1.0			  */
/* Funktion   : Sender kommando til LCD controller, returner fra funktion  	  */
/* 				uden at checke busy flag eller delay				  		  */
/* Input      : 															  */
/* 				unsigned char c indeholder kommando til lcd controller        */
/*                              			                                  */
/* Output	  :															  	  */
/*					 -														  */
/* Kalder	  :																  */
/*				DelayUs()					    						      */
/******************************************************************************/
void lcdWriteInit(unsigned char c)
{
    // Denne funktion anvendes til init sekvens og tester eller venter IKKE
    // på display er klar!!!
    
    RSOff(); RWOff();
    LCD_DIR = 0x00;
    EOn();
    LCD = c;
    DelayNs(500);
    EOff();
    RSOn();
}

/******************************************************************************/
/* Navn       : lcdClear												  	  */
/* Prototype  : void lcdClear(void)	    									  */
/* Programmør : IHN            Dato : 05.05.2018		Version : 2.1  		  */
/* Funktion   :                                                               */
/* 				Sletter LCD og flytter cursor til HOME						  */
/* Input      :															  	  */
/* 				 -															  */
/* Output	  :															  	  */
/*					 -														  */
/* Kalder	  :																  */
/*			    lcdCmd, DelayMs          									  */
/******************************************************************************/
void lcdClear(void)
{
	lcdCmd(CLEAR_DISP);
}

/******************************************************************************/
/* Navn       : lcdHome												    	  */
/* Prototype  : void lcdHome(void)			   								  */
/* Programmør : IHN            Dato : 05.05.2018		Version : 2.1  		  */
/* Funktion   :                                                               */
/* 				Flytter cursor til HOME               						  */
/* Input      :															  	  */
/* 				-															  */
/* Output	  :															  	  */
/*				-														      */
/* Kalder	  :																  */
/*  			lcd_command, delay_ms 									      */
/******************************************************************************/
void lcdHome(void)
{
	lcdCmd(RETURN_HOME);
}

/******************************************************************************/
/* Navn       : lcdGoto     											  	  */
/* Prototype  : void lcdGoto(unsigned char pos)	    						  */
/* Programmør : IHN            Dato : 05.05.2018		Version : 2.1  		  */
/* Funktion   :                                                               */
/* 				Flytter cursor til position pos					  		      */
/* Input      :															  	  */
/* 				unsigned char pos position cursor skal flyttes til (DDRAM adr)*/
/* Output	  :															  	  */
/*			    -		    												  */
/*	Kalder	  :																  */
/*		        lcd_command  												  */
/******************************************************************************/
void lcdGoto(unsigned char pos)
{
	lcdCmd(DDRAM+pos);
}

/******************************************************************************/
/* Navn       : lcdUserChar		  					   					      */
/* Prototype  : void lcdUserChar(char nr, char patterns[8]); 				  */
/* Programmør : IHN            Dato : 05.05.2018		Version : 2.1  		  */
/* Funktion   :                                                               */
/* 				Downloader user karakter til CG ram i LCD, efter download 	  */
/*              placeres cursor i HOME                                        */
/* Input      :															  	  */
/* 				const patterns array med karakter mønster i 5 * 8 matrise     */
/* Output	  :															  	  */
/*					 -														  */
/* Kalder	  :																  */
/*		        lcdWrite, lcdCmd          									  */
/******************************************************************************/
void lcdUserChar(char nr,const char patterns[])
{
	char i;
	lcdCmd(CGRAM+(nr<<3));
	for (i=0; i<8; i++)
	{
		lcdWrite(patterns[i]);
	}
	lcdCmd(DDRAM);
}

/******************************************************************************/
/* Navn       : lcdPrstring 		  					   					  */
/* Prototype  : void lcdPrstring(char *prstring);					 		  */
/* Programmør : IHN            Dato : 05.05.2002		Version : 2.1  		  */
/* Funktion   :                                                               */
/* 				Udskriver en \0 termineret tekststreng på LCD display.	 	  */
/*              placeres cursor i HOME                                        */
/* Input      :															  	  */
/* 				pointer til string											  */
/* Output	  :															  	  */
/*			    -	    													  */
/* Kalder	  :																  */
/*		        lcdCmd          			 								  */
/******************************************************************************/
void lcdPrstring( char *prt)
{
	char *p;
	for (p = prt; *p; p++)
	{
		lcdWrite(*p);
	}
}

/******************************************************************************/
/* OBSOLITE                                                                   */  
/* Navn       : lcdPrintf			  					   					  */
/* Prototype  : void lcd_printf(const char *ptr, ...); 						  */
/* Programmør : IHN            Dato : 05.05.2018		Version : 2.1  		  */
/* Funktion   :                                                               */
/* 				Mini printf funktion til at udskrive på LCD display med 	  */
/*              Følgende printf formateringer er implementeret :              */
/*					 %d														  */
/*					 %s														  */
/*                   %c						                                  */
/*					 %t														  */
/* Input      :															  	  */
/* 				 const string pointer, variabelt antal input parametre        */
/* Output	  :															  	  */
/*					 -														  */
/* Kalder	  :																  */
/*		        lcdWrite, lcdPrstring 		    							  */
/******************************************************************************/
//void lcdPrintf(const char *prt,...)
//{
//	va_list vp;      // Bruges til at pege på de unavngivne argumenter i input
//	char buf[16];
//	const char *p;
//	char cval, *sval;
//	int ival;
//	
//	va_start(vp, prt);	// Peg på første unavngivne argument
//	for (p = prt; *p; p++)
//	{
//		if (*p != '%')
//		{
//			lcdWrite(*p);
//			continue;
//		}
//		switch (*++p)
//		{
//			case 'd' : 	ival = va_arg(vp, int);
//						  	sprintf(buf,"%d", ival);
//						   lcdPrstring(buf);
//						   break;
//			case 'c' :  cval = va_arg(vp,char);
//							lcdWrite(cval);
//							break;
//			case 's' :  sval = va_arg(vp,char *);
//							lcdPrstring(sval);
//							break;
//			case 't' :  ival = va_arg(vp,int);
//							sprintf(buf,"%2d",ival);
//							lcdPrstring(buf);
//							break;							
//			default  :	lcdWrite(*p);
//							break;
//		}
//	}
//	va_end(vp);
//}

void _mon_putc(char c)
{
    lcdWrite(c); // printf() passes each character to write to _mon_putc(), which in turn passes each character to a custom output function
}