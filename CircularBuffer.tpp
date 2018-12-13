/*
 CircularBuffer.tpp - Circular buffer library for Arduino.
 Copyright (c) 2017 Roberto Lo Giacco.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as 
 published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

template<typename T, size_t S, typename IT>
CircularBuffer<T,S,IT>::CircularBuffer() :
	head(0), tail(0), count(0) {
}

template<typename T, size_t S, typename IT>
CircularBuffer<T,S,IT>::~CircularBuffer () {
	clear ();
}

template<typename T, size_t S, typename IT>
CircularBuffer<T,S,IT>::CircularBuffer(const CircularBuffer& src) : head (src.head), tail (src.tail), count (src.count) {
	auto scan = head;
	for (IT i = 0; i < count; ++i) {
		new (&buffer [scan].obj) T (src.buffer [scan].obj);
		if (++scan == capacity)
			scan = 0;
	}
}

template<typename T, size_t S, typename IT>
CircularBuffer<T,S,IT>::CircularBuffer(CircularBuffer&& src) : head (src.head), tail (src.tail), count (src.count) {
	auto scan = head;
	for (IT i = 0; i < count; ++i) {
		new (&buffer [scan].obj) T (static_cast<T&&> (src.buffer [scan].obj));
		if (++scan == capacity)
			scan = 0;
	}
	src.clear ();
}

template<typename T, size_t S, typename IT>
CircularBuffer<T,S,IT>& CircularBuffer<T,S,IT>::operator = (const CircularBuffer& src) {
	clear ();
	head = src.head;
	tail = src.tail;
	count = src.count;
	
	auto scan = head;
	for (IT i = 0; i < count; ++i) {
		new (&buffer [scan].obj) T (src.buffer [scan].obj);
		if (++scan == capacity)
			scan = 0;
	}
	return *this;
}

template<typename T, size_t S, typename IT>
CircularBuffer<T,S,IT>& CircularBuffer<T,S,IT>::operator = (CircularBuffer&& src) {
	clear ();
	head = src.head;
	tail = src.tail;
	count = src.count;
	
	auto scan = head;
	for (IT i = 0; i < count; ++i) {
		new (&buffer [scan].obj) T (static_cast<T&&> (src.buffer [scan].obj));
		if (++scan == capacity)
			scan = 0;
	}
	src.clear ();
	return *this;
}

template<typename T, size_t S, typename IT>
T* CircularBuffer<T,S,IT>::unshift (bool& res) {
	if (head == 0) {
		head = capacity;
	}
	--head;
	if (count == capacity) {
		buffer [head].obj.~T ();
		if (tail-- == 0) {
			tail = capacity - 1;
		}
		res = false;
	} else {
		if (count++ == 0) {
			tail = head;
		}
		res = true;
	}
	return &buffer[head].obj;
}

template<typename T, size_t S, typename IT>
T* CircularBuffer<T,S,IT>::push (bool& res) {
	if (++tail == capacity) {
		tail = 0;
	}
	if (count == capacity) {
		buffer [tail].obj.~T ();
		if (++head == capacity) {
			head = 0;
		}
		res = false;
	} else {
		if (count++ == 0) {
			head = tail;
		}
		res = true;
	}
	return &buffer[tail].obj;
}

template<typename T, size_t S, typename IT>
bool CircularBuffer<T,S,IT>::unshift(const T& value) {
	bool res;
	new (unshift (res)) T (value);
	return res;
}

template<typename T, size_t S, typename IT>
bool CircularBuffer<T,S,IT>::unshift(T&& value) {
	bool res;
	new (unshift (res)) T (static_cast<T&&> (value));
	return res;
}

template<typename T, size_t S, typename IT>
template <typename... Args>
bool CircularBuffer<T,S,IT>::unshift_emplace(Args&&... args) {
	bool res;
	new (unshift (res)) T (static_cast<Args&&> (args)...);
	return res;
}

template<typename T, size_t S, typename IT>
bool CircularBuffer<T,S,IT>::push(const T& value) {
	bool res;
	new (push (res)) T (value);
	return res;
}

template<typename T, size_t S, typename IT>
bool CircularBuffer<T,S,IT>::push(T&& value) {
	bool res;
	new (push (res)) T (static_cast<T&&> (value));
	return res;
}

template<typename T, size_t S, typename IT>
template <typename... Args>
bool CircularBuffer<T,S,IT>::push_emplace(Args&&... args) {
	bool res;
	new (push (res)) T (static_cast<Args&&> (args)...);
	return res;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::shift() {
	if (count <= 0) abort();
	T result (static_cast<T&&> (buffer [head].obj));
	buffer [head].obj.~T ();
	head++;
	
	if (head >= capacity) {
		head = 0;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::pop() {
	if (count <= 0) abort();
	T result (static_cast<T&&> (buffer [tail].obj));
	buffer [tail].obj.~T ();
	
	if (tail == 0) {
		tail = capacity - 1;
	} else {
		--tail;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT>
inline T& CircularBuffer<T,S,IT>::first() {
	return buffer [head].obj;
}

template<typename T, size_t S, typename IT>
inline T& CircularBuffer<T,S,IT>::last() {
	return buffer [tail].obj;
}

template<typename T, size_t S, typename IT>
inline const T& CircularBuffer<T,S,IT>::first() const {
	return buffer [head].obj;
}

template<typename T, size_t S, typename IT>
inline const T& CircularBuffer<T,S,IT>::last() const {
	return buffer [tail].obj;
}

template<typename T, size_t S, typename IT>
T& CircularBuffer<T,S,IT>::operator [](IT index) {
	return buffer [(head + index) % capacity].obj;
}

template<typename T, size_t S, typename IT>
const T& CircularBuffer<T,S,IT>::operator [](IT index) const {
	return buffer [(head + index) % capacity].obj;
}

template<typename T, size_t S, typename IT>
IT inline CircularBuffer<T,S,IT>::size() const {
	return count;
}

template<typename T, size_t S, typename IT>
IT inline CircularBuffer<T,S,IT>::available() const {
	return static_cast<IT> (capacity - count);
}

template<typename T, size_t S, typename IT>
bool inline CircularBuffer<T,S,IT>::isEmpty() const {
	return count == 0;
}

template<typename T, size_t S, typename IT>
bool inline CircularBuffer<T,S,IT>::isFull() const {
	return count == capacity;
}

template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::clear() {
	auto scan = head;
	for (IT i = 0; i < count; ++i) {
		buffer [scan].obj.~T ();
		if (++scan == capacity)
			scan = 0;
	}
	head = tail = 0;
	count = 0;
}

#ifdef CIRCULAR_BUFFER_DEBUG
#include <string.h>
template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::debug(Print* out) const {
	for (IT i = 0; i < capacity; i++) {
		int hex = (int)buffer + i;
		out->print(hex, HEX);
		out->print("  ");
		out->print(*(buffer + i));
		if (head == i) {
			out->print(" head");
		} 
		if (tail == i) {
			out->print(" tail");
		}
		out->println();
	}
}

template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::debugFn(Print* out, void (*printFunction)(Print*, const T&)) const {
	for (IT i = 0; i < capacity; i++) {
		int hex = (int)buffer + i;
		out->print(hex, HEX);
		out->print("  ");
		printFunction(out, *(buffer + i));
		if (head == i) {
			out->print(" head");
		} 
		if (tail == i) {
			out->print(" tail");
		}
		out->println();
	}
}
#endif
