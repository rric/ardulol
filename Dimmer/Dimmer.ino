/* Dimmer.ino
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

// Der analoge Eingangs-Pin des Sensors
const int SensorPin = A0;

// Der Ausgangs-Pin der LED
const int LEDPin = 11;


void setup()
{
    pinMode(SensorPin, INPUT);
    pinMode(LEDPin, OUTPUT);
}


void loop()
{
    // Lies analogen Wert im Bereich [0, 1023] ein, verwandle diesen
    //  in einen Wert im Bereich [0, 255], und gib diesen Wert aus.
    int value = analogRead(SensorPin);
    int pwm = map(value, 0, 1024, 0, 255);

    analogWrite(LEDPin, pwm);

    delay(100);
}

/* IDEEN:
   1. Drehe die Logik um: je niedriger die Helligkeit ist, desto heller 
       soll die LED leuchten.
   2. An welche Pins kann eine "dimmbare" LED angeschlossen werden, und an
      welche nicht? Warum?
*/
