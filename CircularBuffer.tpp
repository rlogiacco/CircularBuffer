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
	head(buffer), tail(buffer), count(0) {
}

template<typename T, size_t S, typename IT>
template <typename Obj>
bool CircularBuffer<T,S,IT>::unshift(Obj&& value) {
	if (head == buffer) {
		head = buffer + capacity;
	}
	--head;
	if (count == capacity) head->obj.~T ();
	new (&head->obj) T (static_cast<Obj&&> (value));
	if (count == capacity) {
		if (tail-- == buffer) {
			tail = buffer + capacity - 1;
		}
		return false;
	} else {
		if (count++ == 0) {
			tail = head;
		}
		return true;
	}
}



template<typename T, size_t S, typename IT>
template <typename Obj>
bool CircularBuffer<T,S,IT>::push(Obj&& value) {
	if (++tail == buffer + capacity) {
		tail = buffer;
	}
	if (count == capacity) tail->obj.~T ();
	new (&tail->obj) T (static_cast<Obj&&> (value));
	if (count == capacity) {
		if (++head == buffer + capacity) {
			head = buffer;
		}
		return false;
	} else {
		if (count++ == 0) {
			head = tail;
		}
		return true;
	}
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::shift() {
	if (count <= 0) abort();
	T result (static_cast<T&&> (head->obj));
	head->obj.~T ();
	head++;
	
	if (head >= buffer + capacity) {
		head = buffer;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::pop() {
	if (count <= 0) abort();
	T result (static_cast<T&&> (tail->obj));
	tail->obj.~T ();
	tail--;
	if (tail < buffer) {
		tail = buffer + capacity - 1;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT>
inline T& CircularBuffer<T,S,IT>::first() {
	return head->obj;
}

template<typename T, size_t S, typename IT>
inline T& CircularBuffer<T,S,IT>::last() {
	return tail->obj;
}

template<typename T, size_t S, typename IT>
T& CircularBuffer<T,S,IT>::operator [](IT index) {
	return (buffer + ((head - buffer + index) % capacity))->obj;
}

template<typename T, size_t S, typename IT>
IT inline CircularBuffer<T,S,IT>::size() {
	return count;
}

template<typename T, size_t S, typename IT>
IT inline CircularBuffer<T,S,IT>::available() {
	return capacity - count;
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
		scan->obj.~T ();
		if (++scan == buffer + capacity)
			scan = buffer;
	}
	head = tail = buffer;
	count = 0;
}

#ifdef CIRCULAR_BUFFER_DEBUG
#include <string.h>
template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::debug(Print* out) {
	for (IT i = 0; i < capacity; i++) {
		int hex = (int)buffer + i;
		out->print(hex, HEX);
		out->print("  ");
		out->print(*(buffer + i));
		if (head == buffer + i) {
			out->print(" head");
		} 
		if (tail == buffer + i) {
			out->print(" tail");
		}
		out->println();
	}
}

template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::debugFn(Print* out, void (*printFunction)(Print*, const T&)) {
	for (IT i = 0; i < capacity; i++) {
		int hex = (int)buffer + i;
		out->print(hex, HEX);
		out->print("  ");
		printFunction(out, *(buffer + i));
		if (head == buffer + i) {
			out->print(" head");
		} 
		if (tail == buffer + i) {
			out->print(" tail");
		}
		out->println();
	}
}
#endif
