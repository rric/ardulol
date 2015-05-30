/* Helligkeit.ino
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

const int SensorPin = A0;


void setup()
{
    Serial.begin(9600);

    pinMode(SensorPin, INPUT);
}


void loop()
{
    int value = analogRead(SensorPin);

    Serial.print("Analoger Wert: "); Serial.println(value);

    delay(100);
}

/* IDEEN:
   1. Erweitere das Programm: sobald die Heligkeit zu niedrig ist, soll
   	  die eingebaute LED aufleuchten.
   2. Zwischen welchen Werten liegt der gemessene Wert? Erweitere das Programm
      so, dass der Wert in den Bereich [0,100] umgerechnet wird.
*/
