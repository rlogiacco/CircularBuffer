#define CIRCULAR_BUFFER_INT_SAFE
#include "CircularBuffer.hpp"
CircularBuffer<unsigned long, 10> timings;


void count() {
  timings.push(millis());
}

void setup() {
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), count, RISING);
}

unsigned long _time = 0;

void loop() {
    Serial.print("buffer size is "); Serial.println(timings.size());
    delay(250);
    if (millis() - _time >= 10000 && !timings.isEmpty()) {
        Serial.print("popping "); Serial.println(timings.pop());
        _time = millis();
    }
}