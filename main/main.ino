/* Gasthon project
 * 
 * main.ino
 *
 * members of the project :
 * 		- YAGHI Anthony
 * 		- SCHAAF Hugo
 * 		- LANDURE Raphaël
 * 
 * Author : SCHAAF Hugo
 *
 * IS3 - ENIB 
 * 01/2019
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

#include "tools.hpp"


#define ONE_WIRE_BUS 3 // one wire sensors bus pin number
#define CANDLE_SENSOR 0 // index of the sensor (1st sensor so index 0)
#define BURNING_TEMP 35. // temperature threshold in deg celcius

// temperature sensor
OneWire oneWire{ONE_WIRE_BUS};
DallasTemperature sensors{&oneWire};

// ventilation management
tools::Ventilation ventilation{10, 11, 3000};

//---------------------------------------------------------------------------//
void setup()
{
	Serial.begin(9600);
	sensors.begin();
	ventilation.begin();
}

//---------------------------------------------------------------------------//
void loop()
{
	static bool force = false;

	// request temperature to the sensor 
	sensors.requestTemperatures();

	// get the temperature above the candle
	float candleTemp = sensors.getTempCByIndex(CANDLE_SENSOR);

	char cmd = Serial.read();

	if(cmd == '1')
	{
		Serial.println("force");
		ventilation.set();
		force = true;
	}

	if(cmd == '2')
	{
		Serial.println("release");
		ventilation.reset();
		force = false;
	}

	if(candleTemp != -127. && !force)
	{
		if(candleTemp >= BURNING_TEMP) ventilation.set();
		else ventilation.reset();
		tools::displayTemp(candleTemp);
	}

	ventilation.run();
}