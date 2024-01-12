#include "CircularBuffer.h"

// Create a circular buffer with a capacity of 10
const int bufferSize = 10;
CircularBuffer<int, bufferSize> buffer;

void setup() {
    // start serial
    Serial.begin(9600);
    while(!Serial); // Wait for the serial port to come online

    // Add some values to the buffer
    for (int i = 0; i < 10; ++i) {
        buffer.push(2*i);
    }

    // Create an array to hold the buffer's contents
    int array[bufferSize];

    // Copy the buffer's contents to the array
    buffer.copyToArray(array);

    // Now array contains the same values as the buffer
    for (int i = 0; i < bufferSize; ++i) {
        Serial.print("Buffer: ");
        Serial.print(buffer[i]); 
        Serial.print(", Array: ");
        Serial.println(array[i]);
    }
}

void loop() {
  // Nothing to do here
}