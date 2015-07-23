/*
  Author: Michal Wolowik
  Corporation: VOLOVIQUE
  Date: Warsaw 23-VII-2015
  Destination: 
    Simple LED blinking project for Pic18fxxkxx devices

  File name:
    main.c

  File destination:
    Main destiny of current code is just blinking
    LED diode with 0.2s period

  Dependecies:
    Software heartbeat located on PB1 port

  Compiler:
    XC8 - version: v1.34 - in PRO mode

  GUI:
    MPLAB X IDE v3.06

  Documentation:
    PIC18F66K80FAMILY DS39977F Revision F Februrary 2012.

  Responsible:
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
		
  Notes:
    Application was tested on AVT-5275v2 development board.
 */



/*
 Language Standard Library:
 */
#include <stdio.h>
#include <stdlib.h>




/*
 Specific Device Library:
 */
#include <p18cxxx.h>
#include <xc.h>
#include <plib.h>



/*
 Configurations Bits Definitions:
 */
#include "Config_Bits.h"




/*
 Clock Definition:
 */
#define MCU_FREQ 64000000




/*
 Port Definitions:
 */
#define LED LATBbits.LATB1





int main(int argc, char** argv) {

    unsigned long int hbeat = 0;
    
    /* Main Oscillator Configuration */
    
    /* Set Internal Oscillator Frequency to 16[MHz] */
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;

    /* Set frequency stable bit */
    OSCCONbits.HFIOFS = 1;

    /* Enable Internal PLL x 4 - MCLK 64 [MHz] */
    OSCTUNEbits.PLLEN = 1;
    
    /* GPIO Ports Configuration */
    
    /* Heartbeat Led diode port configuration */
    TRISBbits.TRISB1 = 0;
    
    while(1){
        
        if(hbeat == 0x17FF){
			LED = 1;
            hbeat++;
		}
		else if( hbeat == 0x8FFF ){
			LED = 0;
            hbeat++;
		}
		else if (hbeat == 0x4FFFF){
			hbeat = 0x00;
        }
        else{			
            hbeat++;
        }
    }
    
    return (EXIT_SUCCESS);
}