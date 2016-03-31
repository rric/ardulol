/* Temperatur.ino
 *
 * Copyright 2013, 2014, 2015, 2016 Roland Richter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/* Measures temperature with the Dallas DS18B20 temperature sensor.
 * Make sure you have both the MAX31850 OneWire and the MAX31850 DallasTemp
 * libraries installed.
 *
 * For instructions how to connect the temperature sensor to Arduino,
 * see http://bildr.org/2011/07/ds18b20-arduino/
 *
 * Writes temperature onto serial, and possibly a serial 7 segment display.
 * See https://learn.sparkfun.com/tutorials/using-the-serial-7-segment-display/
 * for more info.
 */

#include <RBD_Timer.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <SoftwareSerial.h>


// Temperature sensor is connected to pin 2
OneWire oneWire(2);
DallasTemperature sensor(&oneWire);

// Connect LCD shield to pin 8, if any.
SoftwareSerial s7s(7, 8);

RBD::Timer tick;


void setup(void)
{
    tick.setTimeout(6000);

    sensor.begin();

    Serial.begin(9600);
    Serial.println("Time [min], Temperature [C]");

    s7s.begin(9600);

    s7s.write(0x76);  // Clear display command

    s7s.write(0x7A);  // Set brightness command,
    s7s.write(255);   // brightness data byte

    s7s.write(0x77);       // Decimal control command,
    s7s.write(B00000010);  // decimal data byte
}


void loop(void)
{
    if (tick.onRestart()) {
        // Measure temperature in Celsius every six seconds, ...
        double currentTime = ((float)millis()) / (60. * 1000.);

        sensor.requestTemperatures();
        double temperature = sensor.getTempCByIndex(0);

        // ... and output values in CSV form.
        Serial.print(currentTime);
        Serial.print(", ");
        Serial.println(temperature);

        char str[5];

        int value = floor(100 * temperature);
        snprintf(str, 5, "%04d", value);
        s7s.print(str);
    }
}
