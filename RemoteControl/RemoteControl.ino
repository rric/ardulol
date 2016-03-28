/* RemoteControl.ino
 *
 * Copyright 2016 Roland Richter
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

#include <Keypad.h>
#include <IRremote.h>

/*  Verwende Bibliothek Keypad mit entsprechenden Pins, siehe
 *  http://playground.arduino.cc/Main/KeypadTutorial
 *   
 *  '1'  '2'  '3' | 2
 *  '4'  '5'  '6' | 7
 *  '7'  '8'  '9' | 6
 *  '*'  '0'  '#' | 4
 *  --------------+---
 *   3    1    5  |
 * 
 */

const byte Rows = 4;
const byte Cols = 3;

char keys[Rows][Cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[Rows] = {9, 4, 5, 7}; // connect to the row pinouts of the keypad
byte colPins[Cols] = {8, 10, 6};   // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, Rows, Cols);

IRsend irsend;


void setup()
{
    Serial.begin(9600);
}

/* Vivanco Universal Controller UR 12 BN, setting code 1003:
    
    Power:  40BF08F7 (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Play :  40BFB04F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Stop :  40BFD02F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Pause:  40BFF00F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    F >> :  40BFE21D (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    B << :  40BF22DD (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Vol +:  40BF02FD (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Vol -:  40BF42BD (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Ch  +:  40BF52AD (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Ch  -:  40BF12ED (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Up   :  40BF7887 (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Down :  40BFF807 (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Left :  40BF50AF (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    Right:  40BFA25D (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    0    :  40BF00FF (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    1    :  40BF807F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    2    :  40BF40BF (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    3    :  40BFC03F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    4    :  40BF20DF (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    5    :  40BFA05F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    6    :  40BF609F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    7    :  40BFE01F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    8    :  40BF10EF (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
    9    :  40BF906F (NEC, 32 bits) + FFFFFFFF (NEC, 0 bits)
*/

void loop()
{
    char key = keypad.getKey();

    if (key) {
        switch (key) {
            case '1': irsend.sendNEC(0x40BF807F, 32); break;
            case '2': irsend.sendNEC(0x40BF40BF, 32); break;
            case '3': irsend.sendNEC(0x40BFC03F, 32); break;
            case '4': irsend.sendNEC(0x40BF20DF, 32); break;
            case '5': irsend.sendNEC(0x40BFA05F, 32); break;
            case '6': irsend.sendNEC(0x40BF609F, 32); break;
            case '7': irsend.sendNEC(0x40BFE01F, 32); break;
            case '8': irsend.sendNEC(0x40BF10EF, 32); break;
            case '9': irsend.sendNEC(0x40BF906F, 32); break;
            case '*': irsend.sendNEC(0x40BF22DD, 32); break;
            case '#': irsend.sendNEC(0x40BFE21D, 32); break;
        }
    }

    delay(20);
}

