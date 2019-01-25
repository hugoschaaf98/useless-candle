/* Gasthon project
 *
 * tools.hpp
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

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Arduino.h"
#include <Servo.h>

// default delay to enter in emergency mode
#define DELAY 5000 // in ms

namespace tools
{
	//---------------------------------------------------------------------------------------------//

	struct Ventilation
	{
		// constructor
		Ventilation(uint8_t pin0, uint8_t pin1, uint32_t wait_t= DELAY):
		pin_balance{pin0}, pin_orient{pin1}, start_time{0}, wait_t{wait_t},
		emergency_count{false}, doRun{false}
		{
		};

		// run it once to initialize the ventilation system
		void begin()
		{
			servo_balance.attach(pin_balance);
			servo_orient.attach(pin_orient);

			servo_balance.write(0);
			servo_orient.write(90);
		};

		uint8_t run()
		{
			// if no need to ventilate, exit
			if(!doRun) return 0;

#ifdef VERBOSE	
			Serial.println("doRun");
#endif

			if(!emergency_count)
			{
				emergency_count = true;
				start_time = millis();
			}

			if(millis() - start_time > wait_t)
			{
#ifdef VERBOSE	
				Serial.println("emergency");
#endif
				servo_orient.write(0);
			}

			servo_balance.write(40);
			delay(100);
			servo_balance.write(0);
			delay(100);

			return 1;
		};

		void set()
		{
			if(!doRun)
			{

#ifdef VERBOSE	
				Serial.println("set");
#endif
				doRun = true;
			}
		};

		// reset to initial position
		void reset()
		{
			
			if(doRun)
			{
#ifdef VERBOSE	
				Serial.println("reset");
#endif				
				servo_balance.write(0);
				servo_orient.write(90);
				emergency_count = false;
				doRun = false;
			}
		};

		// motorisation members
		Servo servo_balance, servo_orient;
		uint8_t pin_balance, pin_orient;
		// timer members
		int32_t start_time;
		uint32_t wait_t;
		// flags
		bool emergency_count, doRun;
	};

	//---------------------------------------------------------------------------------------------//

	inline
	void displayTemp(float temp)
	{
		static float prev_temp = temp+1.;
		if(temp > prev_temp + 0.3 || temp < prev_temp - 0.3)
		{
			prev_temp = temp;
			Serial.println("candle temperature : "+String(temp)+" °C");
		}
	};

}// namespace tools

#endif