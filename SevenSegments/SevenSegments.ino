/* SevenSegments.ino
 *
 * Copyright 2015 Roland Richter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* Shows numbers and characters on a YSD-439AB4B-35 4-digit 7-segment display;
 * for a data sheet, see e.g. https://www.sparkfun.com/products/9481
 *
 * To turn on and off segments and digits, bits in the PORT[D/B/C] registers
 * are used; for the hows and whys (and the do-nots), see
 * http://www.arduino.cc/en/Reference/PortManipulation.
 *
 * Some analog pins are used for digital output as well; yes, this works,
 * see http://www.arduino.cc/en/Tutorial/AnalogInputPins
 *
 * The following pin mapping is assumed:
 *
 * Arduino Pin   ...   YSD-439AB4B-35 Pin
 *          2           14 (A)
 *          3           16 (B)
 *          4           13 (C)
 *          5            3 (D)
 *          6            5 (E)
 *          7           11 (F)
 *          8           15 (G)
 *          9            1 (DIG1)
 *         10            2 (DIG2)
 *         11            6 (DIG3)
 *         12            8 (DIG4)
 *         A0            7 (Comma)
 *         A1            4 (Colon)
 *
 * Arduino pins 9, 10, 11, and 12 are connected to DIG1, DIG2, DIG3, and DIG4
 * via a 1 kOhm resistor, each (which means "common anode" logic: setting the
 * Arduino pin HIGH turns the correspondig digit OFF, and vice versa).
 * The display's pin 12 is connected to ground via another 1 kOhm resistor;
 * so, setting Arduino pin A1 to HIGH turns the colon ON, and vice versa.
 *
 * For further instructions how to connect and drive the module, see e.g.
 * http://allaboutee.com/2011/07/09/arduino-4-digit-7-segment-display-tutorial/
 */

#include <stdio.h>

#include <Metro.h>


void setup()
{
    for (int port = 2; port <= 12; ++port) {
        pinMode(port, OUTPUT);
    }

    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
}


Metro metro = Metro(1000);
int tick = 0;

void loop()
{
    const int NTicks = 100;

    char str[5];
    snprintf(str, 5, "t=%2d", tick);

    displayString(str);

    if (metro.check()) {
        tick = (++tick % NTicks);
    }
}


// Turn on on of the four digits 1, 2, 3, or 4
void turnOnDigit(int digit)
{
    byte bits = 0;
    bitSet(bits, digit);

    PORTB &= bits | B11100001;
    PORTB |= bits;
}


// Displays the given string, including commas and the colon
void displayString(const char* ch)
{
    for (int i = 0, j = 1; ch[i] != 0; ++i, ++j) {

        // Set segments for the current character
        displayChar(ch[i]);

        // Set comma or colon, if placed correctly
        byte bits = B00000001;

        if (i == 1 && ch[i + 1] == ':') {
            bitSet(bits, 1);
            ++i;
        }
        else if (ch[i + 1] == '.' || ch[i + 1] == ',') {
            bitClear(bits, 0);
            ++i;
        }

        PORTC &= bits | B11111100;
        PORTC |= bits;

        // Turn on the current digit
        turnOnDigit(j);

        delay(2);
    }
}


// Marvel how many characters can be used with this simple display; see also
// http://en.wikipedia.org/wiki/Seven-segment_display_character_representations
void displayChar(char c)
{
    byte bits;

    switch (c) {
        case '-': bits = B01000000; break;
        case '0':
        case 'O': bits = B00111111; break;
        case '1':
        case 'I':
        case 'l': bits = B00000110; break;
        case '2': bits = B01011011; break;
        case '3': bits = B01001111; break;
        case '4': bits = B01100110; break;
        case '5':
        case 'S': bits = B01101101; break;
        case '6': bits = B01111101; break;
        case '7': bits = B00000111; break;
        case '8': bits = B01111111; break;
        case '9': bits = B01101111; break;
        case '=': bits = B01001000; break;
        case 'A': bits = B01110111; break;
        case 'C':
        case '[': bits = B00111001; break;
        case 'E': bits = B01111001; break;
        case 'F': bits = B01110001; break;
        case 'G': bits = B00111101; break;
        case 'H': bits = B01110110; break;
        case 'L': bits = B00111000; break;
        case 'P': bits = B01110011; break;
        case 'T': bits = B00110001; break;
        case 'U': bits = B00111110; break;
        case 'Y': bits = B01101110; break;
        case ']': bits = B00001111; break;
        case '^': bits = B00000001; break;
        case '_': bits = B00001000; break;
        case 'b': bits = B01111100; break;
        case 'c': bits = B01011000; break;
        case 'd': bits = B01011110; break;
        case 'h': bits = B01110100; break;
        case 'i': bits = B00000100; break;
        case 'n': bits = B01010100; break;
        case 'o': bits = B01011100; break;
        case 'r': bits = B01010000; break;
        case 't': bits = B01111000; break;
        case 'u': bits = B00011100; break;
        case ' ':
        default:  bits = B00000000; break;
    }

    // "Common anode" means 1 turns off, and 0 turns on; so negate bits first
    byte negBits = ~bits;

    PORTD &= (negBits << 2) | B00000011;
    PORTD |= (negBits << 2);

    PORTB &= (negBits >> 6) | B11111110;
    PORTB |= (negBits >> 6);
}
