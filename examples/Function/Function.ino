#include <CircularBuffer.hpp>

typedef uint8_t (*func_t)(void);
CircularBuffer<func_t, 10> buffer;

#define SAMPLE_PIN A0

void setup() {
	Serial.begin(9600);
	pinMode(SAMPLE_PIN, INPUT);
	Serial.println("STARTING UP");
}

void loop() {
	unsigned int sample = analogRead(SAMPLE_PIN);
	if (sample < 256) {
		buffer.unshift(first);
	} else if (sample >= 256 && sample < 512) {
		buffer.unshift(second);
	} else if (sample >= 512 && sample < 768) {
		buffer.unshift(third);
	} else {
		buffer.unshift(fourth);
	}
	delay(50);
	if (buffer.isFull()) {
		Serial.println("Queue is full:");
		while (!buffer.isEmpty()) {
			func_t function = buffer.shift();
			function();
		}
		Serial.println("START AGAIN");
	}
}

uint8_t first() {
  Serial.println("doing something...");
  return 0;
}

uint8_t second() {
  Serial.println("does something else...");
  return 1;
}

uint8_t third() {
  Serial.println("maybe this is another thing to do...");
  return 2;
}

uint8_t fourth() {
  Serial.println("how is it going?");
  return 3;
}
