/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControlDC_h
#define LedControlDC_h

#include <avr/pgmspace.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
const static byte charTable [] PROGMEM  = {
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00000000,
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00000000,B00000000,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00010101,B00011101,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

class LedControl {
    private :
        /* The array for shifting the data to the devices */
        byte spidata[16];
        /* Send out a single command to the device */
        void spiTransfer(int addr, byte opcode, byte data);

        /* We keep track of the led-status for all 8 devices in this array */
        byte status[64];
        /* Data is shifted out of this pin*/
        int SPI_MOSI;
        /* The clock is signaled on this pin */
        int SPI_CLK;
        /* This one is driven LOW for chip selectzion */
        int SPI_CS;
        /* The maximum number of devices we use */
        int maxDevices;

    public:
        /* 
         * Create a new controler 
         * Params :
         * dataPin		pin on the Arduino where data gets shifted out
         * clockPin		pin for the clock
         * csPin		pin for selecting the device 
         * numDevices	maximum number of devices that can be controled
         */
        LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);

        /*
         * Gets the number of devices attached to this LedControl.
         * Returns :
         * int	the number of devices on this LedControl
         */
        int getDeviceCount();

        /* 
         * Set the shutdown (power saving) mode for the device
         * Params :
         * addr	The address of the display to control
         * status	If true the device goes into power-down mode. Set to false
         *		for normal operation.
         */
        void shutdown(int addr, bool status);

        /* 
         * Set the number of digits (or rows) to be displayed.
         * See datasheet for sideeffects of the scanlimit on the brightness
         * of the display.
         * Params :
         * addr	address of the display to control
         * limit	number of digits to be displayed (1..8)
         */
        void setScanLimit(int addr, int limit);

        /* 
         * Set the brightness of the display.
         * Params:
         * addr		the address of the display to control
         * intensity	the brightness of the display. (0..15)
         */
        void setIntensity(int addr, int intensity);

        /* 
         * Switch all Leds on the display off. 
         * Params:
         * addr	address of the display to control
         */
        void clearDisplay(int addr);

        /* 
         * Set the status of a single Led.
         * Params :
         * addr	address of the display 
         * row	the row of the Led (0..7)
         * col	the column of the Led (0..7)
         * state	If true the led is switched on, 
         *		if false it is switched off
         */
        void setLed(int addr, int row, int col, boolean state);

        /* 
         * Set all 8 Led's in a row to a new state
         * Params:
         * addr	address of the display
         * row	row which is to be set (0..7)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setRow(int addr, int row, byte value);

        /* 
         * Set all 8 Led's in a column to a new state
         * Params:
         * addr	address of the display
         * col	column which is to be set (0..7)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setColumn(int addr, int col, byte value);

        /* 
         * Display a hexadecimal digit on a 7-Segment Display
         * Params:
         * addr	address of the display
         * digit	the position of the digit on the display (0..7)
         * value	the value to be displayed. (0x00..0x0F)
         * dp	sets the decimal point.
         */
        void setDigit(int addr, int digit, byte value, boolean dp);

        /* 
         * Display a character on a 7-Segment display.
         * There are only a few characters that make sense here :
         *	'0','1','2','3','4','5','6','7','8','9','0',
         *  'A','b','c','d','E','F','H','L','P',
         *  '.','-','_',' ' 
         * Params:
         * addr	address of the display
         * digit	the position of the character on the display (0..7)
         * value	the character to be displayed. 
         * dp	sets the decimal point.
         */
        void setChar(int addr, int digit, char value, boolean dp);
};

// Знакогенератор
const uint8_t characterArrayDC[] PROGMEM = {
//  ABCDEFG  Segments    
  0b11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110, // 9
  0b11101110, // 10  "A"
  0b00111110, // 11  "B"
  0b10011100, // 12  "C"
  0b01111010, // 13  "D"
  0b10011110, // 14  "E"
  0b10001110, // 15  "F"
  0b00000000, // 16  NO DISPLAY
  0b00000000, // 17  NO DISPLAY
  0b00000000, // 18  NO DISPLAY
  0b00000000, // 19  NO DISPLAY
  0b00000000, // 20  NO DISPLAY
  0b00000000, // 21  NO DISPLAY
  0b00000000, // 22  NO DISPLAY
  0b00000000, // 23  NO DISPLAY
  0b00000000, // 24  NO DISPLAY
  0b00000000, // 25  NO DISPLAY
  0b00000000, // 26  NO DISPLAY
  0b00000000, // 27  NO DISPLAY
  0b00000000, // 28  NO DISPLAY
  0b00000000, // 29  NO DISPLAY
  0b00000000, // 30  NO DISPLAY
  0b00000000, // 31  NO DISPLAY
  0b00000000, // 32 ' '
  0b00000000, // 33 '!'  NO DISPLAY
  0b01000100, // 34 '"'
  0b00000000, // 35 '#'  NO DISPLAY
  0b00000000, // 36 '$'  NO DISPLAY
  0b00100100, // 37 '%'  Р—Р°РЅС‡РµРє РїСЂРѕС†РµРЅС‚Р°
  0b00000000, // 38 '&'  NO DISPLAY
  0b01000000, // 39 '''
  0b10011100, // 40 '('
  0b11110000, // 41 ')'
  0b00000000, // 42 '*'  NO DISPLAY
  0b00000000, // 43 '+'  NO DISPLAY
  0b00001000, // 44 ','
  0b00000010, // 45 '-'
  0b00000000, // 46 '.'  NO DISPLAY
  0b00000000, // 47 '/'  NO DISPLAY
  0b11111100, // 48 '0'
  0b01100000, // 49 '1'
  0b11011010, // 50 '2'
  0b11110010, // 51 '3'
  0b01100110, // 52 '4'
  0b10110110, // 53 '5'
  0b10111110, // 54 '6'
  0b11100000, // 55 '7'
  0b11111110, // 56 '8'
  0b11110110, // 57 '9'
  0b00000000, // 58 ':'  NO DISPLAY
  0b00000000, // 59 ';'  NO DISPLAY
  0b00000000, // 60 '<'  NO DISPLAY
  0b00000000, // 61 '='  NO DISPLAY
  0b00000000, // 62 '>'  NO DISPLAY
  0b00000000, // 63 '?'  NO DISPLAY
  0b00000000, // 64 '@'  NO DISPLAY
  0b11101110, // 65 'A'
  0b00111110, // 66 'B'
  0b10011100, // 67 'C'
  0b01111010, // 68 'D'
  0b10011110, // 69 'E'
  0b10001110, // 70 'F'
  0b10111100, // 71 'G'
  0b01101110, // 72 'H'
  0b01100000, // 73 'I'
  0b01110000, // 74 'J'
  0b00000000, // 75 'K'  NO DISPLAY
  0b00011100, // 76 'L'
  0b00000000, // 77 'M'  NO DISPLAY
  0b00101010, // 78 'N'
  0b11111100, // 79 'O'
  0b11001110, // 80 'P'
  0b11100110, // 81 'Q'
  0b00001010, // 82 'R'
  0b10110110, // 83 'S'
  0b00011110, // 84 'T'
  0b01111100, // 85 'U'
  0b00000000, // 86 'V'  NO DISPLAY
  0b00000000, // 87 'W'  NO DISPLAY
  0b00000000, // 88 'X'  NO DISPLAY
  0b01110110, // 89 'Y'
  0b00000000, // 90 'Z'  NO DISPLAY
  0b10011100, // 91 '['
  0b00000000, // 92 '\'  NO DISPLAY
  0b11110000, // 93 ']'
  0b00000000, // 94 '^'  NO DISPLAY
  0b00010000, // 95 '_'
  0b00000100, // 96 '`'
  0b11101110, // 97 'a' SAME AS CAP
  0b00111110, // 98 'b' SAME AS CAP
  0b00011010, // 99 'c'
  0b01111010, // 100 'd' SAME AS CAP
  0b11011110, // 101 'e'
  0b10001110, // 102 'f' SAME AS CAP
  0b10111100, // 103 'g' SAME AS CAP
  0b00101110, // 104 'h'
  0b00100000, // 105 'i'
  0b01110000, // 106 'j' SAME AS CAP
  0b00000000, // 107 'k'  NO DISPLAY
  0b01100000, // 108 'l'
  0b00000000, // 109 'm'  NO DISPLAY
  0b00101010, // 110 'n' SAME AS CAP
  0b00111010, // 111 'o'
  0b11001110, // 112 'p' SAME AS CAP
  0b11100110, // 113 'q' SAME AS CAP
  0b00001010, // 114 'r' SAME AS CAP
  0b10110110, // 115 's' SAME AS CAP
  0b00011110, // 116 't' SAME AS CAP
  0b00111000, // 117 'u'
  0b00000000, // 118 'b'  NO DISPLAY
  0b00000000, // 119 'w'  NO DISPLAY
  0b00000000, // 120 'x'  NO DISPLAY
  0b00000000, // 121 'y'  NO DISPLAY
  0b00000000, // 122 'z'  NO DISPLAY
  0b00000000, // 123 '0b'  NO DISPLAY
  0b00000000, // 124 '|'  NO DISPLAY
  0b00000000, // 125 ','  NO DISPLAY
  0b11000110, // 126 '~'  //Gradus
  0b00000000, // 127 'DEL'  NO DISPLAY
}; 

typedef enum {
   DC_NONE   = 0,
   DC_RED    = 1,
   DC_GREEN  = 2,
   DC_ORANGE = 3,
}DC_COLOR;

class LedControlDC: public LedControl {
  private: 
     char _buffer_r[4];
     char _buffer_g[4];
     void _setValueRed(int addr, int value, uint8_t img);
     void _setValueGreen(int addr, int value, uint8_t img);
     uint8_t _valueToImg( char value, bool dp );
  public:
     LedControlDC(int dataPin, int clkPin, int csPin, int numDevices=1);
     void setChar(int addr, int digit, char value, boolean dp, DC_COLOR color);
     void setCharDC(int addr, int digit, char value_r, boolean dp_r,char value_g, boolean dp_g );
     void clearAnimation(int addr, uint16_t tm);
     void printAnimation(int addr, char v1, char v2, char v3,char v4, boolean dp, DC_COLOR color,uint16_t tm);
     void print(int addr, char v1, char v2, char v3,char v4, boolean dp, DC_COLOR color);
  
};

#endif	//LedControl.h
