// ******************************************************************************************* //
// File:         lab2p1.c
// Date:
// Authors:
//
// Description:
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>

#define DEBOUNCE_DELAY_US 5000


_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeEnum{
    wait,
    scanRows,
    debouncePress,
    debounceRelease,
    displayKey,
} stateType;

volatile stateType currState=wait;
volatile stateType nextState;
volatile char key;

int main(void)
{

    initLCD();
    //printStringLCD("Hello");
    initKeypad();
    int CursorPosition=0;
    while(1){
        switch(currState)
        {
            case wait:
                //IEC1bits.CNIE=1;    //enable cn when we wait for a key press
                break;

            case debouncePress:
                delayUs(DEBOUNCE_DELAY_US);    //delay 5ms
                currState=scanRows;
                break;

            case scanRows:
                //IEC1bits.CNIE=0;    //disable cn when we scan the rows.
                key=scanKeypad();
                currState=wait;
                //IEC1bits.CNIE=1;    //enable cn when we are done scanning the rows.
                break;
                
            case debounceRelease:
                delayUs(DEBOUNCE_DELAY_US);
                currState=displayKey;
                break;

            case displayKey:
                if(CursorPosition==8)   //passed 0x07, the DDRAM address for the first row of LCD.
                {
                    moveCursorLCD(1,0); //move cursor to second row.
                    CursorPosition=40;  //FYI: 0x40 is the DDRAM address for the second row.
                }
                else if(CursorPosition==48) //passed 0x47, the DDRAM address for the second row of LCD.
                {
                    moveCursorLCD(0,0); //move cursor to first row.
                    CursorPosition=0;   //FYI: 0x00 is the DDRAM address for the first row.
                }
                printCharLCD(key);
                CursorPosition++;
                currState=wait;
                break;

        }
                
    }

    return 0;
}

void _ISR _CNInterrupt(void)
{
    IFS1bits.CNIF=0; //put cn flag down.
    if(currState==wait && (COL1PORT==PRESSED || COL2PORT==PRESSED || COL3PORT==PRESSED))
    {
        currState=debouncePress;
    }
    else if(currState==wait && (COL1PORT==RELEASED && COL2PORT==RELEASED && COL3PORT==RELEASED))
    {
        currState=debounceRelease;
    }
}