#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

const byte EXCLAMATION[] = {
0x00, 0x2e
};

const byte QUESTION[] = {
0x00, 0x06, 0x02, 0x2a, 0x0e
};

const byte DOT[] = {
0x00, 0x20
};


const byte blank[] = {
0x00, 0x00, 0x00
};

const byte A[] = {
0x00, 0x3e, 0x0a, 0x3e
};

const byte B[] = {
0x00, 0x3e, 0x2a, 0x2a, 0x14
};

const byte C[] = {
0x00, 0x3e, 0x22, 0x22
};

const byte D[] = {
0x00, 0x3e, 0x22, 0x1c
};

const byte E[] = {
0x00, 0x3e, 0x2a, 0x22
};

const byte F[] = {
0x00, 0x3e, 0x0a, 0x02
};

const byte G[] = {
0x00, 0x3e, 0x22, 0x2a, 0x3a
};

const byte H[] = {
0x00, 0x3e, 0x08, 0x3e
};

const byte I[] = {
0x00, 0x22, 0x3e, 0x22
};

const byte J[] = {
0x00, 0x30, 0x20, 0x3e
};

const byte K[] = {
0x00, 0x3e, 0x08, 0x36
};

const byte L[] = {
0x00, 0x3e, 0x20, 0x20
};

const byte M[] = {
0x00, 0x3e, 0x02, 0x0c, 0x02, 0x3e
};

const byte N[] = {
0x00, 0x3e, 0x0c, 0x18, 0x3e
};

const byte O[] = {
0x00, 0x3e, 0x22, 0x3e
};

const byte P[] = {
0x00, 0x3e, 0x0a, 0x0e
};

const byte Q[] = {
0x00, 0x3e, 0x22, 0x32, 0x3e, 0x20
};

const byte R[] = {
0x00, 0x3e, 0x1a, 0x2e
};

const byte S[] = {
0x00, 0x2e, 0x2a, 0x3a
};

const byte T[] = {
0x00, 0x02, 0x3e, 0x02
};

const byte U[] = {
0x00, 0x3e, 0x20, 0x3e
};

const byte V[] = {
0x00, 0x1e, 0x20, 0x1e
};

const byte W[] = {
0x00, 0x1e, 0x20, 0x18, 0x20, 0x1e
};

const byte X[] = {
0x00, 0x36, 0x08, 0x36
};

const byte Y[] = {
0x00, 0x06, 0x38, 0x06
};

const byte Z[] = {
0x00, 0x32, 0x2a, 0x26
};

const byte ONE[] = {
0x00, 0x22, 0x3e, 0x20
};

const byte TWO[] = {
0x00, 0x3a, 0x2a, 0x2e
};

const byte THREE[] = {
0x00, 0x22, 0x2a, 0x3e
};

const byte FOUR[] = {
0x00, 0x0e, 0x08, 0x3e, 0x08
};


const byte FIVE[] = {
0x00, 0x2e, 0x2a, 0x3a
};

const byte SIX[] = {
0x00, 0x3e, 0x2a, 0x3a
};

const byte SEVEN[] = {
0x00, 0x02, 0x02, 0x3e
};

const byte EIGHT[] = {
0x00, 0x3e, 0x2a, 0x3e
};

const byte NINE[] = {
0x00, 0x0e, 0x0a, 0x3e
};

const byte ZERO[] = {
0x00, 0x3e, 0x22, 0x3e
};


//MODE
#define STACK 1
#define MAZE 2
#define ASTEROID 3
#define MENU 0

#define YELLOW_LED 1
#define BLUE_LED 2
#define UP 3
#define DOWN 4
#define LEFT 5
#define RIGHT 6
#define A_BUTTON 7
#define B_BUTTON 8

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8


#endif