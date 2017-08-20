#include <CircularBuffer.h>

CircularBuffer<char, 10> buffer;

void printBuffer() {
	Serial.print("[");
	for (unsigned int i = 0; i < buffer.size() - 1; i++) {
		Serial.print(buffer[i]);
		Serial.print(",");
	}
	Serial.print(buffer[buffer.size() - 1]);
	Serial.print("] (");

	Serial.print(buffer.size());
	Serial.print("/");
	Serial.print(buffer.size() + buffer.available());
	if (buffer.isEmpty()) {
		Serial.print(" empty");
	} else if (buffer.isFull()) {
		Serial.print(" full");
	}

	Serial.println(")");
}

void setup() {
	Serial.begin(9600);
	buffer.push('a');
	buffer.push('b');
	buffer.push('c');
	buffer.push('d');
	buffer.push('e');
	buffer.push('f');
	buffer.push('g');
	buffer.push('h');
	buffer.push('j');
	buffer.push('k');
	printBuffer(); Serial.println();

	Serial.println("unshift('X')");
	Serial.print(buffer.unshift('X'));
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("push('#')");
	Serial.print(buffer.push('#'));
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("first()");
	Serial.print(buffer.first());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("last()");
	Serial.print(buffer.last());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("pop()");
	Serial.print(buffer.pop());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("pop()");
	Serial.print(buffer.pop());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("shift()");
	Serial.print(buffer.shift());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("shift()");
	Serial.print(buffer.shift());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("clear()");
	buffer.clear();
	Serial.print("buffer is "); 
	printBuffer(); Serial.println();

	Serial.println("unshift('A')");
	Serial.print(buffer.unshift('A'));
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("push('Z')");
	Serial.print(buffer.push('Z'));
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("buffer[5]");
	Serial.print(buffer[5]);
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("buffer[15]");
	Serial.print(buffer[15]);
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("clear()");
	buffer.clear();
	Serial.print("buffer is "); 
	printBuffer(); Serial.println();

	Serial.println("pop()");
	Serial.print(buffer.pop());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();

	Serial.println("shift()");
	Serial.print(buffer.shift());
	Serial.print(" leaving buffer as "); 
	printBuffer(); Serial.println();
}

void loop() {
	
}
