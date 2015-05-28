/* BlinkBlink.ino
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

// Dieser Sketch verwendet die Metro-Bibliothek
// https://github.com/thomasfredericks/Metro-Arduino-Wiring

#include <Metro.h>

// Der Ausgangs-Pin der LED
const int LEDPin = 11;

// Ein Metro-Objekt, das alle 1000 Millisekunden tickt
Metro ledMetro(1000);


void setup()
{
    pinMode(LEDPin, OUTPUT);
}

// Der momentane Status der LED
int state = LOW;

void loop()
{
    // Wechsle bei jedem Tick des LED-Metro zwischen LOW und HIGH
    if (ledMetro.check()) {
        if (state == LOW) {  // Zeile 40
            state = HIGH;
        }
        else {
            state = LOW;
        }

        digitalWrite(LEDPin, state);
    }
}

/* IDEEN:
   1. Baue eine zweite LED ein und erweitere das Programm so, dass diese genau
      dann leuchtet, wenn die erste LED nicht leuchtet (und umgekehrt).

      (Hinweis: dies kann auch durch eine Schaltung erreicht werden.)

   2. Wie klein kann man das Zeitintervall machen, um mit freiem Auge gerade
      noch das Flackern der LED zu erkennen? Was passiert, wenn man das
      Zeitintervall noch kleiner macht?

   3. Finde mindestens zwei weitere Möglichkeiten, wie man in Zeile 40 ff
      zwischen LOW und HIGH hin- und her wechseln kann.
      (Hinweis: http://stackoverflow.com/questions/13154073)

   4*. Wie lange dauert die Ausführung des Befehls digitalWrite() im
      Durchschnitt? (Hinweis: verwende http://arduino.cc/en/Reference/Micros)
*/
