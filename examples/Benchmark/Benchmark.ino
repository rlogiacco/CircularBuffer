#include <CircularBuffer.h>

volatile uint8_t dummyByte = 5;

// Make a template object that is size bytes
template<uint8_t size>
struct Obj
{
  Obj()
  {
    for (uint8_t i = 0; i < size; i++)
    {
      buf[i] = dummyByte++;
    }
  }

  uint8_t buf[size];

  bool operator!=(Obj<size> &rhs)
  {
    for (uint8_t i = 0; i < size; i++)
    {
      if (buf[i] != rhs.buf[i])
      {
        return true;
      }
    }

    return false;
  }

  Obj<size> operator=(uint8_t rhs)
  {
    buf[0] = rhs;
  }

  operator uint8_t()
  {
    return buf[0];
  }
};

template<typename T, typename TIn>
void testPush()
{
  uint16_t start, end;

  CircularBuffer<T, 10, uint8_t, TIn> buf;

  T a, b;
  
  noInterrupts();
  start = TCNT1;
  buf.push(a);
  end = TCNT1;
  interrupts();

  b = buf.pop();
  if (b != a)
  {
    Serial.println("Error!");
  }

  // Prevent compiler from optimizing things out
  dummyByte = b.buf[0];

  // End - start is the number of clock cycles that the operation took,
  // plus the number of cycles to read the counter, which is usually around
  // 2 to 4 cycles, depending on how the compiler chooses to compile this
  Serial.print(end - start);
  Serial.print(",");
}

// Recursively define template to test pushing objects of size 1 through size
template<uint8_t size>
void testPushRef()
{
  testPushRef<size - 1>();
  testPush<Obj<size>, const Obj<size> &>();
}

template<>
void testPushRef<0>() {}

template<uint8_t size>
void testPushVal()
{
  testPushVal<size - 1>();
  testPush<Obj<size>, const Obj<size>>();
}

template<>
void testPushVal<0>() {}





template<typename T>
void testPopByRefImpl()
{
  uint16_t start, end;

  CircularBuffer<T, 10, uint8_t, T> buf;

  T a, b;
  buf.push(a);
  
  noInterrupts();
  start = TCNT1;
  buf.pop(b);
  end = TCNT1;
  interrupts();

  if (b != a)
  {
    Serial.println("Error!");
  }

  // Prevent compiler from optimizing things out
  dummyByte = b.buf[0];

  // End - start is the number of clock cycles that the operation took,
  // plus the number of cycles to read the counter, which is usually around
  // 2 to 4 cycles, depending on how the compiler chooses to compile this
  Serial.print(end - start);
  Serial.print(",");
}

// Recursively define template to test pushing objects of size 1 through size
template<uint8_t size>
void testPopRef()
{
  testPopRef<size - 1>();
  testPopByRefImpl<Obj<size>>();
}

template<>
void testPopRef<0>() {}




template<typename T>
void testPopByValImpl()
{
  uint16_t start, end;

  CircularBuffer<T, 10, uint8_t, T> buf;

  T a, b;
  buf.push(a);
  
  noInterrupts();
  start = TCNT1;
  b = buf.pop();
  end = TCNT1;
  interrupts();

  if (b != a)
  {
    Serial.println("Error!");
  }
  
  // Prevent compiler from optimizing things out
  dummyByte = b.buf[0];

  // End - start is the number of clock cycles that the operation took,
  // plus the number of cycles to read the counter, which is usually around
  // 2 to 4 cycles, depending on how the compiler chooses to compile this
  Serial.print(end - start);
  Serial.print(",");
}

template<uint8_t size>
void testPopVal()
{
  testPopVal<size - 1>();
  testPopByValImpl<Obj<size>>();
}

template<>
void testPopVal<0>() {}






void setup() {
	Serial.begin(9600);

  // Set Timer 1 in normal counting mode, at the clock speed
  // This lets us count the individual clock cycles for each operation
  TCCR1A = 0;
  TCCR1B = 1;

  Serial.println();
  Serial.println("Push By Value Timings");
  testPushVal<16>();
  
  Serial.println();
  Serial.println("Push By Reference Timings");
  testPushRef<16>();

  Serial.println();
  Serial.println("Pop By Value Timings");
  testPopVal<16>();
  
  Serial.println();
  Serial.println("Pop By Reference Timings");
  testPopRef<16>();

  Serial.println();

  // Output on a Arduino Mega clone
  // Push By Value Timings
  // 64,68,72,78,166,181,210,227,250,267,292,298,321,336,359,374,
  // Push By Reference Timings
  // 59,60,64,68,86,120,127,134,145,148,155,162,169,176,183,190,
  // Pop By Value Timings
  // 47,53,61,73,209,221,247,268,301,324,347,359,380,401,422,443,
  // Pop By Reference Timings
  // 37,41,45,49,67,90,97,104,111,118,125,132,139,146,153,160,
}

void loop() {
}
