/* SevenSegments.ino
 *
 * Copyright 2015, 2016 Roland Richter
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
 *          5           14 (A)
 *          6           16 (B)
 *          7           13 (C)
 *          8            3 (D)
 *          9            5 (E)
 *         10           11 (F)
 *         11           15 (G)
 *         12            7 (Comma)
 *         13            4 (Colon)
 *         A0            8 (DIG4)
 *         A1            6 (DIG3)
 *         A2            2 (DIG2)
 *         A3            1 (DIG1)
 *
 * Arduino pins A0, A1, A2 and A3 are each connected to DIG4, DIG3, DIG2, 
 * and DIG1 via a 1 kOhm resistor, which means "common anode" logic: 
 * setting the Arduino pin HIGH turns the digit OFF, and vice versa.
 * 
 * The only "common cathode" logic digit is the colon: Arduino pin 13 is
 * connected to segment pin 4, and segment pin 12 is connected to ground
 * via a 1 kOhm resistor.
 * So, setting Arduino pin 13 to HIGH turns the colon ON, and vice versa.
 *
 * For further instructions how to connect and drive the module, see e.g.
 * http://allaboutee.com/2011/07/09/arduino-4-digit-7-segment-display-tutorial/
 */

#include <stdio.h>
#include <RBD_Timer.h>

RBD::Timer timer;
int tick = 0;


void setup()
{
    for (int port = 5; port <= 13; ++port) {
        pinMode(port, OUTPUT);
    }

    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);

    timer.setTimeout(1000);
    timer.restart();

    while (timer.isActive()) {
        displayString("Ardu");
    }

    timer.restart();

    while (timer.isActive()) { 
        displayString(" ino");
    }

    timer.setTimeout(100);
}


void loop()
{
    const int NTicks = 99;

    // For some reasons, Arduino's sprintf() implementation does not support %f, 
    // so do a workaround here. See also http://stackoverflow.com/q/27651012
    char str[6];
    snprintf(str, 6, "t=%02d", tick);
    str[4] = str[3];
    str[3] = '.';
    str[5] = 0;

    displayString(str);

    if (timer.onRestart()) {
        tick = (++tick % NTicks);
    }
}


// Turn on or of the four digits 1, 2, 3, or 4
void turnOnDigit(int digit)
{
    byte bits = 0;
    bitSet(bits, 4-digit);

    // Zero out pins A0 to A3, then set the one which was set in bits
    PORTC &= bits | B11110000;
    PORTC |= bits;
}


// Displays the given string, including commas and the colon
void displayString(const char* ch)
{
    for (int i = 0, j = 1; ch[i] != 0; ++i, ++j) {

        // Set segments for the current character
        displayChar(ch[i]);

        // Set comma or colon, if placed correctly
        boolean showComma = false;
        boolean showColon = false;

        if (i == 1 && ch[i + 1] == ':') {
            showColon = true;
            ++i;
        }
        else if (ch[i + 1] == '.' || ch[i + 1] == ',') {
            showComma = true;
            ++i;
        }
        // Comma follows "common anode" logic, 
        // whereas colon follows "common cathode" logic
        digitalWrite(12, showComma ? LOW : HIGH);
        digitalWrite(13, showColon ? HIGH : LOW);

        // Turn on the current digit
        turnOnDigit(j);

        delay(2);
    }
}


// Many characters can be used with this simple display; see also
// http://en.wikipedia.org/wiki/Seven-segment_display_character_representations
void displayChar(char c)
{
    byte bits;

    // Set the seven bits for segements A to G, in G-F-E-D-C-B-A order
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

    PORTD &= (negBits << 5) | B0001111;
    PORTD |= (negBits << 5);

    PORTB &= (negBits >> 3) | B11110000;
    PORTB |= (negBits >> 3);
}
