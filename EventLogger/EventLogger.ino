/* EventLogger.ino
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


/* Dieser Entwurf registriert Ereignisse, die von einem Interrupt ausgelöst
 * wurden, und speichert diese in den EEPROM-Speicher ab.
 *
 * Statt der Standard-Bibliothek wird EEPROMEx verwendet:
 * http://thijs.elenbaas.net/2012/07/extended-eeprom-library-for-arduino/
 *
 * Download:
 * http://thijs.elenbaas.net/files/EEPROMEx-9.1.zip
 */

#include <EEPROMex.h>

const int BarrierPin = 2;
const int BuzzPin   = 12;

const unsigned int MagicNumber = 31293;

// Interrupt service routine
volatile boolean newAlarm = false;
volatile unsigned long alarmTime = 0;

void triggerAlarm()
{
    newAlarm = true;
    alarmTime = millis();
}


int address = 0;
unsigned int nSavedAlarms = 0;

void setup()
{
    Serial.begin(9600);

    pinMode(BarrierPin, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BuzzPin, OUTPUT);

    attachInterrupt(0, triggerAlarm, RISING);   // Interrupt 0 = Pin 2 auf UNO

    // Eine "magische Zahl" in den ersten beiden Bytes zeigt an, ob
    // bereits gültige Daten im EEPROM liegen oder noch nicht.
    //
    // Die Daten werden in folgender Form abgepeichert:
    //
    // Adresse 0 - 1: magische Zahl (unsigned int)
    // Adresse 2 - 3: Anzahl der gespeicherten Einträge (unsigned int)
    // Adresse 4 - 7: erster Eintrag (unsigned long)
    //         8 - 11: zweiter Eintrag (unsigned long)
    //         ...
    //         n - n+3: letzter Eintrag (unsigned long)

    if (EEPROM.readInt(0) != MagicNumber) {
        EEPROM.writeInt(0, MagicNumber);

        nSavedAlarms = 0;
        EEPROM.writeInt(2, 0);
    }
    else {
        nSavedAlarms = EEPROM.readInt(2);
    }

    // Reserviere Platz für 128 Einträge
    EEPROM.setMemPool(2 * sizeof(unsigned int), 128 * sizeof(unsigned long));
    EEPROM.setMaxAllowedWrites(100);

    Serial.print(nSavedAlarms);
    Serial.println(" Alarme gespeichert:");

    for (unsigned int i = 0; i < nSavedAlarms; ++i) {
        address = EEPROM.getAddress(sizeof(unsigned long));

        Serial.print(i); Serial.print(": ");
        Serial.println(EEPROM.readLong(address));
    }
}


void loop()
{
    // Falls neuer Alarm: speichere Anzahl sowie Zeitpunkt
    //               und schalte Ton für vier Sekunden ein
    if (newAlarm) {
        ++nSavedAlarms;
        EEPROM.updateInt(2, nSavedAlarms);
        address = EEPROM.getAddress(sizeof(unsigned long));
        EEPROM.writeLong(address, alarmTime);

        Serial.print("Neuer Alarm ");
        Serial.print(nSavedAlarms - 1); Serial.print(": ");
        Serial.println(alarmTime);

        tone(BuzzPin, 880);
        digitalWrite(LED_BUILTIN, HIGH);

        delay(4000);

        noTone(BuzzPin);
        digitalWrite(LED_BUILTIN, LOW);

        newAlarm = false;
    }

    delay(50);
}

/* IDEEN:
   1. Erweitere die Ausnahmebehandlung: Wenn zu viele (d.h. mehr als 128)
      Alarme auftreten, dann sollen nur die Daten der letzten 128 Ereignisse
      gespeichert werden. Ältere Daten können überschrieben werden.
   2. millis() gibt die Zeit ab Start des Programms zurück. Falls der Prozessor
      zwischendurch neu gestartet wird (z.B. wegen Stromausfalls), können die
      Zeiten einzelner Ereignisse nicht mehr miteinander verglichen werden.
      Wie könnte man dieses Problem lösen?
   3. Erweitere das Programm so, dass ein Administrator nach Eingabe eines
      Passworts gespeicherte Daten löschen kann.
*/

