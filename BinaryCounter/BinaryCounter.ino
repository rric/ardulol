/* BinaryCounter.ino
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
 
#include <SoftwareSerial.h>

#include <RBD_Timer.h>
#include <RBD_Button.h>


// Taster an Pin 2 und Pin 3
RBD::Button increment(2);
RBD::Button decrement(3);

// Pin der ersten LED
const int LEDPin = 11;

// Serial 7-Segment Display an Pin 12
SoftwareSerial s7s(3, 12);


RBD::Timer latency;
char str[6];

// Zähler mit oder ohne Vorzeichen
byte counter = 0;
/* int8_t counter = 0; */



void setup()
{
    // Pins 4 bis 11 sind OUTPUT
    for (int pos = 0; pos < 8; ++pos) {
        pinMode(LEDPin - pos, OUTPUT);
    }

    s7s.begin(9600);
    s7s.write(0x76); // Clear display command

    latency.setHertz(6);
}


void loop()
{
    // Falls Taste gedrückt ist, ändere Zähler sechs Mal pro Sekunde
    if (latency.isExpired()) {
        if (increment.isPressed()) {
            ++counter;
            latency.restart();
        }
        else if (decrement.isPressed()) {
            --counter;
            latency.restart();
        }
    }

    // Zeige Zähler als Binärzahl an
    for (int pos = 0; pos < 8; ++pos) {
        digitalWrite(LEDPin - pos, bitRead(counter, pos) ? HIGH : LOW);
    }

    sprintf(str, "%4d", counter);
    s7s.print(str);

    delay(20);
}

