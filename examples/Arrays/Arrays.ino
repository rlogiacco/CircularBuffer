#include "CircularBuffer.hpp"

// Create a circular buffer with a capacity of 10
const int bufferSize = 10;
CircularBuffer<int, bufferSize> buffer;


char convert(const int& i) {
    return (char)i;
}

void setup() {
    // start serial
    Serial.begin(9600);
    while(!Serial); // Wait for the serial port to come online

    // Add some values to the buffer
    for (int i = 65; i < 65 + 6; i++) {
        buffer.push(i);
    }
    
    Serial.print("Size:");
    Serial.println(buffer.size());

    // Create an array to hold the buffer's contents
    int subArray[buffer.size()];

    // Copy the buffer's contents to the array
    buffer.copyToArray(subArray);

    // Now array contains the same values as the buffer
    for (int i = 0; i < buffer.size(); ++i) {
        Serial.print("Buffer: ");
        Serial.print(buffer[i]); 
        Serial.print(", Array: ");
        Serial.println(subArray[i]);
    }

    // Add some more values to the buffer
    for (int i = 65 + 6; i < 65 + 14; i++) {
        buffer.push(i);
    }

    Serial.print("Size:");
    Serial.println(buffer.size());

    // Create an array to hold the buffer's contents
    int array[buffer.size()];

    // Copy the buffer's contents to the array
    buffer.copyToArray(array);

    // Now array contains the same values as the buffer
    for (int i = 0; i < buffer.size(); ++i) {
        Serial.print("Buffer: ");
        Serial.print(buffer[i]); 
        Serial.print(", Array: ");
        Serial.println(array[i]);
    }

    // Create an array to hold the buffer's contents as chars
    char chars[buffer.size()];

    // Copy and convert the buffer's contents to the array
    buffer.copyToArray(chars, convert);

     // Now array contains the same values as the buffer
    for (int i = 0; i < buffer.size(); ++i) {
        Serial.print("Buffer: ");
        Serial.print((char)buffer[i]); 
        Serial.print(", Array: ");
        Serial.println(chars[i]);
    }
}

void loop() {
  // Nothing to do here
}
