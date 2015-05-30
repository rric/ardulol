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

/* Liest analoge Werte eines Sharp GP2Y0A21YK IR Distanzsensors ein und
 * gibt diese auf einem LED-Balken aus -- je näher, desto mehr LEDs leuchten.
 */

int SensorPin = A0;

void setup()
{
    Serial.begin(9600);
    
    // Pins 3 bis 12 sind OUTPUT
    for (int pos = 3; pos <= 12; ++pos) {
        pinMode(pos, OUTPUT);
    }
}


void loop()
{
    // Lies analogen Wert im Bereich [0, 1023] ein, verwandle diesen
    // in einen Wert im Bereich [0, 10], ...
    int value = analogRead(SensorPin);
    int out = map(value, 0, 1024, 0, 10);  // Zeile 37
    int show = constrain(out, 0, 10);

    // ... und schalte entsprechend viele LEDs ein
    for (int pos = 0; pos < 10; ++pos) {
        digitalWrite(3 + pos, pos < show ? HIGH : LOW);
    }
    
    // Gib Werte über serielle Schnittstelle aus
    Serial.print(value); 
    Serial.print(" "); 
    Serial.println(show);
    
    delay(100);
}


/* IDEEN:
    1. Passe die Werte in Zeile 37 so an, dass bei kleiner Distanz alle
        10 LEDs leuchten.
    
    2*. Bestimme eine Funktion, die die analogen Werte im Bereich [0, 1023] 
        in einen Wert in Zentimetern im Bereich [0 cm, 100 cm] umrechnet 
        Hinweis: http://www.sparkfun.com/datasheets/Components/GP2Y0A21YK.pdf
*/
