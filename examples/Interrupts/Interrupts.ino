#define CIRCULAR_BUFFER_INT_SAFE
CircularBuffer<volatile long, 10> times;

void setup() {
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), count, RISING);
}

unsigned long time = 0;

void loop() {
    Serial.print("buffer count is "); Serial.println(times.count());
    delay(250);
    if (millis() - time >= 10000 && !times.isEmpty()) {
        Serial.print("popping "); Serial.println(times.pop());
        time = millis();
    }
}

void count() {
  times.push(millis());
}