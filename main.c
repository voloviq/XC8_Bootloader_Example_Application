/*
  Author: Michal Wolowik
  Corporation: VOLOVIQUE
  Date: Warsaw 20-VIII-2015
  Destination: 
    Simple LEDs blinking project for Pic18fxxkxx devices.
    Software is desired to be used with bootloader application
    which resieds in 0-0x0FFF in flash loaction and user application
    starts from 0x1000 in flash location.

  File name:
    main.c

  File destination:
    Main destiny of current code is just blinking
    LED_1 diode with about 0.1s period and LED_2 about 0.5s period.

  Dependecies:
    Software heartbeat located on PB1(in main while loop) 
    and PB2(in interrupt routine) port. This software start
    form 0x1000 location in flash (used codeoffset option in
    linker configuration).

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
#include <plib/timers.h>




/*
 Configurations Bits Definitions:
 */
#include "Config_Bits.h"




/*
 Clock Definition - 64MHz:
 */
#define MCU_FREQ 64000000




/*
 LEDs port defines:
 */
#define LED_1 LATBbits.LATB1
#define LED_2 LATBbits.LATB2




/*
 Timer 0 default value to reload
 */
#define TIMER0_DEF_VALUE 0xF000





/*
 GPIO ports defines:
 */
#define SET_PORT_AS_OUTPUT 0




/*
Function name   :   low_isr
Description     :   Common interrupt routine
Input 1         :   None
Output          :   None
Function return :   None
Testing result  :   Not fully tested yet
Test conditions :   AVT-5275v2 - MCU Clock 64MHz
Optymalization  :   For XC8 used - 'PRO mode'
Function valid  :   Function invalid - not tested yet
Function rev.   :   V1_0
Approved by     :   not approved yet
Issue date      :   20.VIII.2015
Problem report  :   None
*/
void interrupt low_isr(void);




void main(void){

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
    
    /* 
     Timer0 configuration: 
     TIMER_INT_ON  - Enable TIMER Interrupt
     T0_16BIT      - Timer0 is configured as an 16-bit timer/counter
     T0_SOURCE_INT - Internal instruction cycle clock (CLKO) acts as source of clock
     T0_PS_1_256   - 1:256 Prescale value
     */
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
    
    /* Reload Timer 0 default value */
    WriteTimer0(TIMER0_DEF_VALUE);
    
    /* Clear timer interrupt flage */
    INTCONbits.TMR0IF = 0;

    /* Activate global interrupt system */
    ei();
    
    /* 
     GPIO Ports Configuration - both as output:
     LED_1 - connected to PORTB.1
     LED_2 - connected to PORTB.2
     */
    
    /* Heartbeat LED_1 and LED_2 diodes - port configuration (Output mode for both) */
    TRISBbits.TRISB1 = SET_PORT_AS_OUTPUT;
    TRISBbits.TRISB2 = SET_PORT_AS_OUTPUT;
    
    while(1){
        if(hbeat == 0x17FF){
			LED_1 = 1;
            hbeat++;
		}
		else if( hbeat == 0x8FFF ){
			LED_1 = 0;
            hbeat++;
		}
		else if (hbeat == 0x4FFFF){
			hbeat = 0x00;
        }
        else{			
            hbeat++;
        }
    }
}




void interrupt low_isr(void){
    /* Check if overflow interrupt for Timer 0 occures */
    if(INTCONbits.TMR0IF == 1){
        /* Cyclic change state from ON to OFF and so on */
        LED_2 = ~LED_2;
        /* Clear timer interrupt flage - it's necessary */
        INTCONbits.TMR0IF = 0;
        /* Upload default value to timer */
        WriteTimer0(TIMER0_DEF_VALUE);
    }
}