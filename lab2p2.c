/*lab2p2.c
 Author: Adley Gin
 ECE372
 */

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define DEBOUNCE_DELAY_US 5000
#define ENTER 0
#define SET 1

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeEnum{
    enter,
    wait,
    setmode,
    good,
    bad,
    invalid,
    valid,
    scanRows,
    debouncePress,
    debounceRelease,
    displayKey,
} stateType;

volatile stateType currState=enter;
volatile char currPW[5];
volatile char pw1[5], pw2[5], pw3[5], pw4[5];
volatile int mode=ENTER;    //mode=0 means "enter" state. mode=1 means "set mode" state.
volatile char key;
volatile int j=0;

void storePassword(char c[]);
int checkPassword(char c[]);

int main(void)
{
    int i=0;
    char* ptr;
    initLCD();
    initKeypad();
    while(1)
    {
        switch(currState)
        {
            case enter:
                mode=ENTER;
                memset(currPW,0,4);
                printStringLCD("Enter");
                moveCursorLCD(1,0);     //immediately move cursor to second row of LCD.
                currState=wait;
                break;

            case wait:

                break;

            case debouncePress:
                delayUs(DEBOUNCE_DELAY_US);    //delay .5ms
                currState=scanRows;
                break;

            case scanRows:
                key=scanKeypad();   //grab the key that was pressed on the keypad.
                currPW[strlen(currPW)]=key; //add the key pressed onto the current string of keys pressed.
                currState=wait;
                break;

            case debounceRelease:
                delayUs(DEBOUNCE_DELAY_US);
                currState=displayKey;
                break;

            case displayKey:
                printCharLCD(key);
                if(mode==ENTER)
                {
                    if(strcmp(currPW,"**")==0)
                    {
                        currState=setmode;
                    }
                    else if(strlen(currPW)==4 && (strcmp(currPW,pw1)==0 || strcmp(currPW,pw2)==0 ||
                        strcmp(currPW,pw3)==0 || strcmp(currPW,pw4)==0))
                    {
                        currState=good;
                    }
                    else if(strlen(currPW)==4 && (strcmp(currPW,pw1)!=0 && strcmp(currPW,pw2)!=0 &&
                        strcmp(currPW,pw3)!=0 && strcmp(currPW,pw4)!=0))
                    {
                        currState=bad;
                    }
                    else
                    {
                        currState=wait;
                    }
                }

                else if(mode==SET)
                {
                    if(strlen(currPW)==4 && strtol(currPW,&ptr,10)!=0) //valid password. Store it.
                    {
                        currState=valid;
                    }

                    else if(strlen(currPW)==4 && strtol(currPW,&ptr,10)==0)    //invalid password. Don't store it.
                    {
                        currState=invalid;
                    }

                    else
                    {
                        currState=wait;
                    }
                }


                break;

            case setmode:
                mode=SET;
                memset(currPW,0,4);
                clearLCD();
                moveCursorLCD(0,0);
                printStringLCD("Set Mode");
                moveCursorLCD(1,0);
                currState=wait;
                break;

            case good:
                moveCursorLCD(0,0);
                printStringLCD("Good!");
                for(i=0; i<450; i++) //delay 2 seconds
                {
                    delayUs(65000); //max delay is 4.44ms. Need to loop 450 times to reach 2 seconds.
                }
                clearLCD();
                currState=enter;
                break;

            case bad:
                moveCursorLCD(0,0);
                printStringLCD("BAD!!!");
                for(i=0; i<450; i++) //delay 2 seconds
                {
                    delayUs(65000); //max delay is 4.44ms. Need to loop 450 times to reach 2 seconds.
                }
                clearLCD();
                currState=enter;
                break;

            case valid:
                storePassword(currPW);
                moveCursorLCD(0,0);
                printStringLCD("        ");
                moveCursorLCD(0,0);
                printStringLCD("Valid!");
                for(i=0; i<450; i++)
                {
                    delayUs(65000);
                }
                clearLCD();
                currState=enter;
                break;

            case invalid:
                moveCursorLCD(0,0);
                printStringLCD("INVALID!");
                for(i=0; i<450; i++)
                {
                    delayUs(65000);
                }
                clearLCD();
                currState=enter;
                break;


        }


    }

    return 0;
}

void _ISR _CNInterrupt()
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

void storePassword(char c[])
{
    if(j>3)
    {
        j=0;
    }
    if(j==0)
    {
        strcpy(pw1,c);
    }
    else if(j==1)
    {
        strcpy(pw2,c);
    }
    else if(j==2)
    {
        strcpy(pw3,c);
    }
    else if(j==3)
    {
        strcpy(pw4,c);
    }
    j++;
}

int checkPassword(char c[])
{
    int result=1;
    int i=0;

    for(i=0; i<4; i++)
    {
        if(isdigit(c[i]==0))
        {
            result=0;
            return result;
        }
    }


    return result;
}