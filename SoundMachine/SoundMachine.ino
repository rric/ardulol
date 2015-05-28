/* SoundMachine.ino
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

/* Frequenzen der Töne (auf ganze Zahlen gerundet) nach
 * https://de.wikipedia.org/wiki/Frequenzen_der_gleichstufigen_Stimmung
 * beginnend mit C1, D1, ..., d.h.
 * C1 -> FreqC[0], A4 -> FreqA[3] usw.
 */

const int FreqC[7]  = { 33,  65, 131, 262, 523, 1046, 2093};
const int FreqCs[7] = { 35,  69, 139, 277, 554, 1109, 2217};
const int FreqD[7]  = { 37,  73, 147, 294, 587, 1175, 2349};
const int FreqDs[7] = { 39,  78, 156, 311, 622, 1245, 2489};
const int FreqE[7]  = { 41,  82, 165, 330, 659, 1319, 2637};
const int FreqF[7]  = { 44,  87, 175, 349, 698, 1397, 2794};
const int FreqFs[7] = { 46,  92, 185, 370, 740, 1480, 2960};
const int FreqG[7]  = { 49,  98, 196, 392, 784, 1568, 3136};
const int FreqGs[7] = { 52, 104, 208, 415, 831, 1661, 3322};
const int FreqA[7]  = { 55, 110, 220, 440, 880, 1760, 3520};
const int FreqAs[7] = { 58, 117, 233, 466, 932, 1865, 3729};
const int FreqB[7]  = { 62, 123, 247, 494, 988, 1976, 3951};

const int BuzzPin = 12;


void playNote(int pin, char note, boolean sharp, char height, int duration)
{
    int h = height - '1';

    if (0 <= h && h <= 7) {
        int freq = 0;

        switch (note) {
            case 'C': freq = (sharp ? FreqCs[h] : FreqC[h]);
                break;
            case 'D': freq = (sharp ? FreqDs[h] : FreqD[h]);
                break;
            case 'E': freq = FreqE[h];
                break;
            case 'F': freq = (sharp ? FreqFs[h] : FreqF[h]);
                break;
            case 'G': freq = (sharp ? FreqGs[h] : FreqG[h]);
                break;
            case 'A': freq = (sharp ? FreqAs[h] : FreqA[h]);
                break;
            case 'B': freq = FreqB[h];
                break;
        }

        tone(pin, freq);
    }

    delay(duration);
    noTone(pin);
}


void setup()
{
    Serial.begin(9600);

    pinMode(BuzzPin, OUTPUT);
}


char ch[3];
int n = 0;

void loop()
{
    if (Serial.available() > 0) {
        char c = Serial.read();

        boolean isNote = (n == 0 && ('A' <= c && c <= 'G'));
        boolean isSharp = (n == 1 && c == '#');
        boolean isHeight = (n == 1 || n == 2) && ('1' <= c && c <= '7');

        boolean ok = isNote || isSharp || isHeight;

        if (ok) {
            ch[n] = c;

            if (isHeight) {
                playNote(BuzzPin, ch[0], (ch[1] == '#'), ch[n], 400);
                n = 0;
            }
            else {
                ++n;
            }
        }
        else {
            n = 0;
        }
    }
}

/* Teste mit verschiedenen Musikstücken, z.B. mit
   D4D4E4D4G4F#4 D4D4E4D4A4G4 D4D4D5B4 G4G4F4E4C5 C5B4G4A4G4
   C4D4E4F4G4G4 A4A4A4A4G4 A4A4A4A4G4 F4F4F4F4E4E4 D4D4D4D4C4
*/

/* IDEEN:
   1. Erweitere das Programm: ein Musikstück, das in einer Zeile übergeben
      wurde, soll so lange wiederholt werden, bis eine neue Zeile übergeben
      wird.
   2. Erweitere dieses "Protokoll" so, dass man auch die Länge jeder Note
      sowie ein Zeichen für eine Pause mit übergeben kann.
*/
