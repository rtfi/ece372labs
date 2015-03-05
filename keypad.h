/*
 * File:   keypad.h
 * Author: user
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#define COL1TRIS TRISBbits.TRISB8    //left column
#define COL2TRIS TRISBbits.TRISB5    //middle column
#define COL3TRIS TRISBbits.TRISB10    //right column

#define ROW1TRIS TRISAbits.TRISA0
#define ROW2TRIS TRISAbits.TRISA1
#define ROW3TRIS TRISBbits.TRISB2
#define ROW4TRIS TRISBbits.TRISB3

#define COL1PORT PORTBbits.RB8      //defined port bits for input.
#define COL2PORT PORTBbits.RB5
#define COL3PORT PORTBbits.RB10

#define PRESSED 0       //for use with port bits.
#define RELEASED 1      //for use with port bits.

void initKeypad(void);
char scanKeypad(void);
void scanRow(int row);

#endif	/* KEYPAD_H */
