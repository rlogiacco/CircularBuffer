CircularBuffer [![Build Status][travis-status]][travis]
=============
[travis]: https://travis-ci.org/rlogiacco/CircularBuffer
[travis-status]: https://travis-ci.org/rlogiacco/CircularBuffer.svg?branch=master

By default the library uses `unsigned int` indexes, allowing for a maximum of 65536 items, but you'll rarely need such a huge store. Defining the `CIRCULAR_BUFFER_XS` macro you can reduce the library indexes to `unsigned short` with no actual impact on the memory used by the library itself, but allowing you to squeeze out those few extra bytes whenever you perform indexed access, if you do any. Obviously the consequence is having a maximum element capacity of 512 items, still plenty in most cases.

The library itself has an implicit memory consumption of about *0.5Kb*: 580b (max) of code and 8b of memory, to my calculations. That does not consider the space used to store the items themselves, obviously.

Usage
============

### Declare and initialize

When declaring your buffer you should specify the data type it must handle and the buffer capacity: those two parameters will influence the memory consumed by the buffer.

``` cpp
#include <CircularBuffer.h>

CircularBuffer<short,100> shorts; // uses 538 bytes 
CircularBuffer<int,100> ints; // uses 638 bytes
CircularBuffer<long,100> longs; // uses 838 bytes
CircularBuffer<float,100> floats; // uses 988 bytes
CircularBuffer<double,100> doubles; // uses 988 bytes
CircularBuffer<char,100> chars; // uses 538 bytes 
CircularBuffer<void*,100> pointers; // uses 638 bytes
```

If you are close to using all your memory you can try to squeeze out a few bytes by defining `CIRCULAR_BUFFER_XS` **BEFORE** including the library:

``` cpp
#define CIRCULAR_BUFFER_XS
#include <CircularBuffer.h>

CircularBuffer<short,100> buffer;
```

Please note the reduced memory usage will not occur unless you were using the array-like access operator **AND** you start using `unsigned int` data type for the index access. 


### Store data

Let's start making things clear: the library doesn't support inserting data in the middle of the buffer.
You can add data to the buffer either before the first element via an `unshift()` operation or after the last element via a `push()` operation.
You can keep adding data beyond the buffer maximum capacity, but you'll lose the least significant information:

* since `unshift()` adds to the _head_, adding beyond capacity causes the element at _tail_ to be overwritten and lost
* since `push()` adds to the _tail_, adding beyond capacity causes the element at _head_ to be overwritten and lost

Both `unshift()` and `push()` return `true` if the addition didn't cause any information loss, `false` if an overwrite occurred:

``` cpp
CircularBuffer<int,5> buffer; // buffer capacity is 5

// all of the following return true
buffer.unshift(1); // [1] 
buffer.unshift(2); // [2,1]
buffer.unshift(3); // [3,2,1]
buffer.push(0);  // [3,2,1,0]
buffer.push(5);  // [3,2,1,0,5]

buffer.unshift(2);  // [2,3,2,1,0] returns false
buffer.unshift(10); // [10,2,3,2,1] returns false
buffer.push(-5);  // [2,3,2,1,-5] returns false
```

### Retrieve data

Similarly to data addition, data retrieval can be performed at _tail_ via a `pop()` operation or from _head_ via an `unshift()` operation: both cause the element being read to be removed from the buffer.
Reading from an empty buffer is forbidden (the library will generate a segfault, which most probably will crash the program): see the _additional operations_ listed in the next section to verify the status of the buffer.

Non-destructive read operations are also available:

* `first()` returns the element at _head_
* `last()` returns the element at _tail_
* an array-like indexed read operation is also available so you can read any element in the buffer using the `[]` operator

Reading data beyond the actual buffer size has an undefined behaviour and is user's responsibility to prevent such boundary violations using the _additional operations_ listed in the next section.

``` cpp
CircularBuffer<char, 50> buffer; // ['a','b','c','d','e','f','g']

buffer.first(); // ['a','b','c','d','e','f','g'] returns 'a'
buffer.last(); // ['a','b','c','d','e','f','g'] returns 'g'
buffer.pop(); // ['a','b','c','d','e','f'] returns 'g'
buffer.pop(); // ['a','b','c','d','e'] returns 'f'
buffer.shift(); // ['b','c','d','e'] returns 'a'
buffer.shift(); // ['c','d','e'] returns 'b'
buffer[0]; // ['c','d','e'] returns 'c'
buffer[1]; // ['c','d','e'] returns 'd'
buffer[2]; // ['c','d','e'] returns 'e'

buffer[10]; // ['c','d','e'] returned value is unpredictable
buffer[15]; // ['c','d','e'] returned value is unpredictable
```

### Additional operations

* `isEmpty()` returns `true` only if no data is stored in the buffer
* `isFull()` returns `true` if no data can be further added to the buffer without causing overwrites/data loss
* `size()` returns the number of elements currently stored in the buffer; it should be used in conjunction with the `[]` operator to avoid boundary violations: the first element index is always `0` (if buffer is not empty), the last element index is always `size() - 1`
* `available()` returns the number of elements that can be added before saturating the buffer
* `capacity()` returns the number of elements the buffer can store, for completeness only as it's user defined and never changes
* `clear()` resets the whole buffer to its initial state removing all elements


------------------------
### CHANGE LOG

#### 1.1.0

* Improved robustness against access outside the buffer boundaries
* Fixed `pop()` and `shift()` implementations
* Added test sketch
* Added `capacity()` function
* Added `debug()` function, disabled by pre processor by default

#### 1.0.0

* Initial implementation