/* Lichtfalle.ino
 *
 * Copyright 2015, 2017 Roland Richter
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

const int BarrierPin = 2;
const int BuzzPin   = 12;

volatile boolean alarm = false;

// Interrupt service routine
void triggerAlarm()
{
    alarm = true;
}


void setup()
{
    pinMode(BarrierPin, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BuzzPin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(BarrierPin), triggerAlarm, RISING);   // Zeile 38
                                                // Interrupt 0 = Pin 2 auf UNO
}


void loop()
{
    if (alarm) {
        tone(BuzzPin, 880);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
        noTone(BuzzPin);
        digitalWrite(LED_BUILTIN, LOW);
    }

    delay(50);
}

/* IDEEN:
   1. Warum wird in Zeile 38 der Interrupt mit einer steigenden Flanke ("RISING")
      verknüpft? Was würde passieren, wenn man hier "FALLING" oder "CHANGE"
      verwenden würde?
   2. Erweitere das Programm so, dass der Alarm beendet wird, sobald die
      Lichtschranke nicht mehr unterbrochen ist.
   3. Ändere den Alarm: so lange die Lichtschranke unterbrochen ist, soll
      die LED blinken und den Ton parallel dazu ein- und ausgeschaltet werden.
*/
