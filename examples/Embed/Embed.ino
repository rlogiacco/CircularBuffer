#include <Sensor.cpp>

Sensor<100> sensor;

unsigned long _time = 0;

#define SAMPLE_PIN A0

void setup() {
	Serial.begin(9600);
	pinMode(SAMPLE_PIN, INPUT);
	_time = millis();
}

void loop() {
	sensor.update(analogRead(A0));
    sleep(100);
    Serial.print(".");
	if (millis() - _time >= 1000) {
		_time = millis();
        Serial.println();
		
		Serial.print("Minimum seen value is ");
		Serial.println(sensor.min());

		Serial.print("Maximum seen value is ");
		Serial.println(sensor.max());

		Serial.print("Average on ");
		Serial.print(sensor.count());
		Serial.print(" samples is ");
		Serial.println(sensor.avg());	
    }
}
