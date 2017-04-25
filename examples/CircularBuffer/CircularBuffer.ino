#include <CircularBuffer.h>

CircularBuffer<int, 100> buffer;

unsigned long time = 0;

void setup() {
	Serial.begin(9600);
	pinMode(A0, INPUT);
	time = millis();
}

// samples A0 and prints the average of the latest hundred samples to console every 500ms

void loop() {
	int reading = analogRead(A0);
	buffer.append(reading);

	if (millis() - time >= 500) {
		time = millis();
		float avg = 0.0;
		for (unsigned int i = 0; i < buffer.size(); i++) {
			avg += buffer[i] / buffer.size();
		}
		Serial.print("Average is ");
		Serial.println(avg);
	}
}
