/* 
 * File:    lcdDrv.h
 * Author:  IHN
 * Version: 2.1 
 * Created on 11. april 2018, 10:55
 */


#ifndef LCDDRV_H
#define	LCDDRV_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/////////// Opsætning af driver  ///////////
// I driveren kan der vælges mellem at benytte busy flag eller bruge delay
// for at sikre at display er klar til at modtage en ny kommando defineres
// BUSYFLAG vil der blive benyttet tjek af busyflaget.
//#define CHECKBUSY

/////////// Include statement ///////////
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
    
/////////// Microchip Harmony specifikke headerfiler  ///////////
#include "system_config.h"
#include "system_definitions.h"

#include "Delay.h"
    
/////////// Globale funktioner ////////////
extern void lcdInit(unsigned char mode);
extern void lcdWrite(unsigned char c);
extern void lcdClear(void);
extern void lcdHome(void);
extern void lcdGoto(unsigned char pos);
//For redirecting printf output
void _mon_putc(char c);

//extern void lcdLedOn(void);
//extern void lcdLedOff(void);
extern void lcdUserChar(char nr, const char patterns[8]);
extern void lcdPrstring( char *prt);
//extern void lcdPrintf(const char *prt,...);
//extern void lcdLed(unsigned char on);
////////////   Lokale funtioner     ////////////
unsigned char lcdRead();
void lcdCmd(unsigned char c);
void lcdWriteInit(unsigned char c);

//////////// HAL   ////////////

//////////// Processor specifikke definitioner ////////////

#ifndef RSOn
    #error Husk at definere RSOn()
#else
    #warning Bruger Harmony implementation af RSOn() 
#endif
#ifndef RSOff
    #error Husk at definere RSOff()
#else
    #warning Bruger Harmony implementation af RSOff() 
#endif

#ifndef EOn
    #error Husk at definere EOn()
#else
    #warning Bruger Harmony implementation af EOn() 
#endif
#ifndef EOff
    #error Husk at definere EOff()
#else
    #warning Bruger Harmony implementation af EOff() 
#endif

#ifndef RWOn
    #error Husk at definere RWOn()
#else
    #warning Bruger Harmony implementation af RWOn() 
#endif
#ifndef RWOff
    #error Husk at definere RWOff()
#else
    #warning Bruger Harmony implementation af RWOff() 
#endif

#ifndef LCD
	#define LCD 		PORTE			// Data port opsætte manuelt
	#define LCD_DIR 	TRISE
#endif

// Definer KONTRAST og KONTRASTDir hvis der ønske at kunne styre 
// kontrast med en PWM udgang
//#define KONTRAST     RC2
//#define KONTRASTDir  TRISC2

#define OUTPUT 	0
#define INPUT  	1

//////////// LCD relaterede definitioner ////////////
// Kommandoer
#define CLEAR_DISP	  1
#define RETURN_HOME	  2
#define ENTRY_MODE	  4
#define DISP_ONOFF	  8
#define CURSOR_DISP	 16
#define FUNCTION_SET 32
#define SET_CG_ADR	 64
#define SET_DD_ADR	128

// Cursor
#define NO  		  0
#define BLINK  		  1 
#define CURSOR 		  2
#define CURSORBLINK   3

// Memory
#define CGRAM 	   0x40
#define DDRAM      0x80

// Opsætning
#define SETUP      0x38
#define INIT       0x30
#define BUSY	   0x80

// Display
#define DISPON     0b00001100
#define MOVE	   0x06

// Linie adresser
#define LINIE1	   0x00
#define LINIE2	   0x40
#define LINIE3	   0x14
#define LINIE4	   0x54

////////////      Generelle definitioner       ////////////
#ifndef TRUE
	#define TRUE	  1
#endif
#ifndef FALSE
   #define FALSE      0
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* LCDDRV_H */

