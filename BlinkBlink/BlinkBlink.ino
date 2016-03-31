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

// Dieser Sketch verwendet die RBD::Timer-Bibliothek
#include <RBD_Timer.h>

// Der Ausgangs-Pin der LED
const int LEDPin = 11;

// Der Timer zum Schalten der LED
RBD::Timer ledTimer;

void setup()
{
    pinMode(LEDPin, OUTPUT);

    ledTimer.setTimeout(1000);
}

// Der momentane Status der LED
int state = LOW;

void loop()
{
    // Wechsle mit jedem Restart zwischen LOW und HIGH
    if (ledTimer.onRestart()) {

        if (state == LOW) {  // Zeile 43
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

   3. Finde mindestens zwei weitere Möglichkeiten, wie man in Zeile 43 ff
      zwischen LOW und HIGH hin- und her wechseln kann.
      (Hinweis: http://stackoverflow.com/questions/13154073)

   4*. Wie lange dauert die Ausführung des Befehls digitalWrite() im
      Durchschnitt? (Hinweis: verwende http://arduino.cc/en/Reference/Micros)
*/
