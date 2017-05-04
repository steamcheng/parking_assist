/*
 * Arduino sketch using the Adafruit version of the ST VL53L0X time of flight
 * laser distance sensor to control a red / yellow / green light to assist in 
 * parking in a tight garage.
 * 
 * Uses the Adafruit packaged breakout board for the VL53L0X sensor and the
 * Pololu VL53L0X library for Arduino.
 * 
 * Created by Davis Whittaker on 4 May 2017
 * 
 * So far, compiled but not tested with hardware.
 */

#include <Wire.h>
#include <VL53L0X.h>

// Red, yellow, green lights
#define redpin 10
#define yellowpin  11
#define greenpin  12

int distance = 2000;
int last_distance = 2000;
int ctr = 0;

VL53L0X sensor;


/****************Debug Setting********************/

// boolean debug = true;   // Set for debug on
 boolean debug = false;  // Set for debug off

void setup()
{
	if (debug) { Serial.begin(115200); } // Open serial monitor at 115200 baud to see results.
	Wire.begin();

	sensor.init();
	sensor.setTimeout(500);

	// Start continuous back-to-back mode (take readings as
	// fast as possible).  To use continuous timed mode
	// instead, provide a desired inter-measurement period in
	// ms (e.g. sensor.startContinuous(100)).
	sensor.startContinuous(100);

	// Set up R/G/B light pins
	pinMode(redpin, OUTPUT);      // Red light
	pinMode(yellowpin, OUTPUT);   // Yellow light
	pinMode(greenpin, OUTPUT);    // Red light
	// Set all lights off at start
	digitalWrite(redpin, LOW);
	digitalWrite(yellowpin, LOW);
	digitalWrite(greenpin, LOW);
}

void loop()
{
	if (debug) { Serial.println(sensor.readRangeContinuousMillimeters()); }
	if (sensor.timeoutOccurred()) {
		if (debug) { Serial.println(" TIMEOUT"); }
		distance = last_distance;
/*		ctr++;
		if (ctr == 5){
			sensor.init();
			sensor.setTimeout(500);
			sensor.startContinuous();
			ctr = 0;
			if (debug) { Serial.println(" RESET Sensor"); } } */
	}
	else distance = sensor.readRangeContinuousMillimeters();


	/* Check distance in several ranges and turn on appropriate lights. */
	if (distance > 1000 || distance == 0) {
		digitalWrite(greenpin, LOW);
		digitalWrite(yellowpin, LOW);
		digitalWrite(redpin, LOW);
                if (debug) { Serial.println(" OFF"); }
	}

	if (distance > 610 && distance < 915) { // Green
		greenlight();
                if (debug) { Serial.println(" Green"); }
	}

	if (distance > 460 && distance <= 610) { // Green-Yellow
		greenyellow();
                if (debug) { Serial.println(" Green-Yellow"); }
	}

	if (distance > 305 && distance <= 460) { // Yellow-Red
		yellowred();
                if (debug) { Serial.println(" Yello-Red"); }
	}

//	if (distance <= 305 && distance != 0) { // Red
	if (distance <= 305 && distance > 203) { // Red
		redlight();
                if (debug) { Serial.println(" Red"); }
	}

//	if (distance <= 203 && distance > 152 && distance != 0) {
  	if (distance <= 203 && distance > 152) {
		flashred(250);
                if (debug) { Serial.println(" Slow"); }
	}

//	if (distance <= 152 && distance != 0) {
  	if (distance <= 152) {
		flashred(125);
                if (debug) { Serial.println(" Too Close!"); }
	}

	last_distance = distance;


} // End main loop



void redlight(void){
	digitalWrite(greenpin, LOW);
	digitalWrite(yellowpin, LOW);
	digitalWrite(redpin, HIGH);
}

void yellowred(void){
	digitalWrite(greenpin, LOW);
	digitalWrite(yellowpin, HIGH);
	digitalWrite(redpin, HIGH);
}

void greenyellow(void){
	digitalWrite(greenpin, HIGH);
	digitalWrite(yellowpin, HIGH);
	digitalWrite(redpin, LOW);
}

void greenlight(void){
	digitalWrite(greenpin, HIGH);
	digitalWrite(yellowpin, LOW);
	digitalWrite(redpin, LOW);
}

/* Function to flash red light if too close. */
void flashred(int d){
	digitalWrite(redpin, HIGH);
	delay(d);
	digitalWrite(redpin, LOW);
} // End flashred()

