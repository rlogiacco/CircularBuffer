CircularBuffer [![Build Status][travis-status]][travis]
=============
[travis]: https://travis-ci.org/rlogiacco/CircularBuffer
[travis-status]: https://travis-ci.org/rlogiacco/CircularBuffer.svg?branch=master

By default the library uses `unsigned int` indexes, allowing for a maximum of 65536 items, but you'll rarely need such a huge store. Defining the `CIRCULAR_BUFFER_XS` macro you can reduce the library indexes to `unsigned short` with no actual impact on the memory used by the library itself, but allowing to squeez out those few extra bytes whenever you perform indexed access, if you do any. Obviously the consequence is having a maximum element capacity of 512 items, still plenty in most cases.

The library itself has an implicit memory consumption of about *0.5Kb*: 580b (max) of code and 8b of memory, to my calculations. That doesn not consider the space used to store the items themselves, obviously.

Usage
============

1. Declare and initialize
---------------------

When declaring your buffer you should specify the data type it must handle and the buffer capacity: those two parameters will influence the memory consumed by the buffer.

```
CircularBuffer<short,100> shorts; // uses 538 bytes 
CircularBuffer<int,100> ints; // uses 638 bytes
CircularBuffer<long,100> longs; // uses 838 bytes
CircularBuffer<float,100> floats; // uses 988 bytes
CircularBuffer<double,100> doubles; // uses 988 bytes
CircularBuffer<char,100> chars; // uses 538 bytes 
CircularBuffer<void*,100> pointers; // uses 638 bytes
```

2. Store data
------------------------


3. Retrieve data
------------------------


4. Additional operations
------------------------

* `clear()` 
