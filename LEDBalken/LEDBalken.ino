/* Lichtbalken.ino
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

int PotPin = A0;

void setup()
{
    // Pins 3 bis 12 sind OUTPUT
    for (int pos = 0; pos < 10; ++pos) {
        pinMode(3 + pos, OUTPUT);
    }
}


void loop()
{
    // Lies analogen Wert im Bereich [0, 1023] ein, verwandle diesen
    // in einen Wert im Bereich [0, 10], ...
    int value = analogRead(PotPin);
    int show = map(value, 0, 1023, 0, 10);

    // ... und schalte entsprechend viele LEDs ein
    for (int pos = 0; pos < 10; ++pos) {
        digitalWrite(3 + pos, pos < show ? HIGH : LOW);
    }
}


/* IDEEN:
    1. Ändere die Anzeige so, dass immer nur eine LED aufleuchtet und je nach
    analogem Wert hin- und herwandert.
*/
