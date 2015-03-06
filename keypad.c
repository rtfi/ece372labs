#include "p24FJ64GA002.h"
#include "keypad.h"
#include "timer.h"
#include "string.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    ODCAbits.ODA0 =1;   //RA0 (row1) odc enabled.
    ODCAbits.ODA1=1;    //RA1 (row2) odc enabled.
    ODCBbits.ODB2=1;    //RB2 (row3) odc enabled.
    ODCBbits.ODB3=1;    //RB3 (row4) odc enabled.
    

    ROW1TRIS=0; //RA0 set as output.
    ROW2TRIS=0; //RA1 set as output.
    ROW3TRIS=0; //RB2 set as output.
    ROW4TRIS=0; //RB3 set as output.
  


    COL1TRIS=1; //RB8 set as input.
    COL2TRIS=1; //RB5 set as input.
    COL3TRIS=1; //RB10 set as input.

    CNPU2bits.CN16PUE=1;    //pull-up resistor enabled for RB10.
    CNPU2bits.CN22PUE=1;    //pull up resistor enabled for RB8.
    CNPU2bits.CN27PUE=1;    //pull-up resistor enabled for RB5.

    IFS1bits.CNIF=0;
    IEC1bits.CNIE=1;    //global cn enabled

    CNEN2bits.CN16IE=1; //turn on cn for RB10.
    CNEN2bits.CN22IE=1; //turn on cn for RB8.
    CNEN2bits.CN27IE=1; //turn on cn for RB5.


    
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
char scanKeypad(void){
    char key = 'a';

   scanRow(1);
   if(COL1PORT==PRESSED)
         return key='1';
    else if(COL2PORT==PRESSED)
        return key='2';
    else if(COL3PORT==PRESSED)
        return key='3';
    else
    {
        key='b';
        return key;
    }
   

    scanRow(2);
    if(COL1PORT==PRESSED)
         key='4';
    else if(COL2PORT==PRESSED)
         key='5';
    else if(COL3PORT==PRESSED)
         key='6';
    else
    {
        key='c';
        return key;
    }

    scanRow(3);
    if(COL1PORT==PRESSED)
         key='7';
    else if(COL2PORT==PRESSED)
         key='8';
    else if(COL3PORT==PRESSED)
         key='9';
    else
    {
        key='d';
        return key;
    }

    scanRow(4);

    if(COL1PORT==PRESSED)
         key='*';
    else if(COL2PORT==PRESSED)
         key='0';
    else if(COL3PORT==PRESSED)
         key='#';
    else
    {
        key='e';
        return key;
    }
    
    return key;
}

void scanRow(int row)
{
    IEC1bits.CNIE = 0;
    if(row==1)
    {
        ROW1LAT=1;
        ROW2LAT=0;
        ROW3LAT=0;
        ROW4LAT=0;
    }
    else if(row==2)
    {
        ROW1LAT=0;
        ROW2LAT=1;
        ROW3LAT=0;
        ROW4LAT=0;
    }
    else if(row==3)
    {
        ROW1LAT=0;
        ROW2LAT=0;
        ROW3LAT=1;
        ROW4LAT=0;
    }
    else if(row==4)
    {
        ROW1LAT=0;
        ROW2LAT=0;
        ROW3LAT=0;
        ROW4LAT=1;
    }
    IEC1bits.CNIE = 1;
}

