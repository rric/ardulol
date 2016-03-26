/* RemoteLEDShow.ino
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

#include <RBD_Timer.h>
#include <IRremote.h>

boolean shiftLeft = true;

byte leftMost = 2;
byte rightMost = 0;

byte hertz = 5;

RBD::Timer tick;

IRrecv irrecv(2);
decode_results results;


void setup()
{
    Serial.begin(9600);

    // Pins 3 bis 13 sind OUTPUT
    for (byte pos = 0; pos < 10; ++pos) {
        pinMode(3 + pos, OUTPUT);
    }

    tick.setHertz(hertz);

    irrecv.enableIRIn();
}


void loop()
{
    if (tick.onRestart()) {
        if (shiftLeft) {
            leftMost  = (leftMost + 1) % 10;
            rightMost = (rightMost + 1) % 10;
        }
        else {
            leftMost  = (leftMost + 10 - 1) % 10;
            rightMost = (rightMost + 10 - 1) % 10;
        }

        // ... und schalte entsprechend viele LEDs ein
        for (byte pos = 0; pos < 10; ++pos) {

            boolean turnOn = (leftMost >= rightMost)
                             ? (leftMost >= pos && pos >= rightMost)
                             : (pos >= rightMost || leftMost >= pos);

            digitalWrite(3 + pos, turnOn ? HIGH : LOW);
        }
    }

    if (irrecv.decode(&results)) {
        Serial.print(results.value, HEX);
        Serial.print(" (");

        switch (results.decode_type) {
            case NEC:  Serial.print("NEC"); break;

            case SONY: Serial.print("SONY"); break;

            case RC5:  Serial.print("RC5"); break;

            case RC6:  Serial.print("RC6"); break;

            case UNKNOWN:
            default: Serial.print("UNKNOWN"); break;
        }

        Serial.print(", ");
        Serial.print(results.bits);
        Serial.println(" bits)");

        if (results.decode_type == NEC && results.bits == 32) {

            switch (results.value) {
                case 0x40BF02FD: // Vol +: schneller
                    if (hertz < 50) {
                        ++hertz;
                        tick.setHertz(hertz);
                    }

                    break;

                case 0x40BF42BD: // Vol -: langsamer
                    if (hertz > 1) {
                        --hertz;
                        tick.setHertz(hertz);
                    }

                    break;

                case 0x40BFE21D: case 0x40BFA25D: // F >>, Right
                    shiftLeft = true;
                    break;

                case 0x40BF22DD: case 0x40BF50AF: // B <<, Left
                    shiftLeft = false;
                    break;

                case 0x40BF807F: // "1"
                    rightMost = (leftMost + 10 - 0) % 10;
                    break;

                case 0x40BF40BF: // "2"
                    rightMost = (leftMost + 10 - 1) % 10;
                    break;

                case 0x40BFC03F: // "3"
                    rightMost = (leftMost + 10 - 2) % 10;
                    break;

                case 0x40BF20DF: // "4"
                    rightMost = (leftMost + 10 - 3) % 10;
                    break;

                case 0x40BFA05F: // "5"
                    rightMost = (leftMost + 10 - 4) % 10;
                    break;

                case 0x40BF609F: // "6"
                    rightMost = (leftMost + 10 - 5) % 10;
                    break;

                case 0x40BFE01F: // "7"
                    rightMost = (leftMost + 10 - 6) % 10;
                    break;

                case 0x40BF10EF: // "8"
                    rightMost = (leftMost + 10 - 7) % 10;
                    break;

                case 0x40BF906F: // "9"
                    rightMost = (leftMost + 10 - 8) % 10;
                    break;
            }
        }

        irrecv.resume();
    }

    delay(5);
}
