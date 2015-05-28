/* Taster.ino
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

// Der Eingangs-Pin des Tasters
const int ButtonPin = 2;

// Der Ausgangs-Pin der LED
const int LEDPin = 11;


void setup()
{
    pinMode(ButtonPin, INPUT);
    pinMode(LEDPin, OUTPUT);
}


void loop()
{
    int state = digitalRead(ButtonPin);
    digitalWrite(LEDPin, state);

    delay(10);
}


/* IDEEN:
   1. Erweitere das Programm so, dass mit Druck auf die Taste zwischen den
      drei Zuständen LED dunkel -- LED blinkt -- LED hell gewechselt wird.
   2. Erweitere das Programm so, dass die Zeit des Tastendrucks gemssen wird.
      Die LED soll danach genau im Intervall der gemessenen Zeit blinken.
*/
