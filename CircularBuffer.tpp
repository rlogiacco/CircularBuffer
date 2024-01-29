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

template<typename T, size_t S, typename IT, typename TIn>
constexpr CircularBuffer<T,S,IT, TIn>::CircularBuffer() :
		head(buffer), tail(buffer), count(0) {
}

template<typename T, size_t S, typename IT, typename TIn>
bool CircularBuffer<T,S,IT,TIn>::unshift(TIn value) {
	if (head == buffer) {
		head = buffer + capacity;
	}
	*--head = value;
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

template<typename T, size_t S, typename IT, typename TIn>
bool CircularBuffer<T,S,IT, TIn>::push(TIn value) {
	if (++tail == buffer + capacity) {
		tail = buffer;
	}
	*tail = value;
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

template<typename T, size_t S, typename IT, typename TIn>
T CircularBuffer<T,S,IT, TIn>::shift() {
	if (count == 0) return *head;
	T result = *head++;
	if (head >= buffer + capacity) {
		head = buffer;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT, typename TIn>
void CircularBuffer<T,S,IT, TIn>::shift(T &result) {
	if (count == 0) {
		result = *head;
	} else {
		result = *head++;
		if (head >= buffer + capacity) {
			head = buffer;
		}
		count--;
	}
}

template<typename T, size_t S, typename IT, typename TIn>
T CircularBuffer<T,S,IT, TIn>::pop() {
	if (count == 0) return *tail;
	T result = *tail--;
	if (tail < buffer) {
		tail = buffer + capacity - 1;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT, typename TIn>
void CircularBuffer<T,S,IT, TIn>::pop(T &result) {
	if (count == 0) {
		result = *tail;
	} else {
		result = *tail--;
		if (tail < buffer) {
			tail = buffer + capacity - 1;
		}
		count--;
	}
}

template<typename T, size_t S, typename IT, typename TIn>
T inline CircularBuffer<T,S,IT, TIn>::first() const {
	return *head;
}

template<typename T, size_t S, typename IT, typename TIn>
void inline CircularBuffer<T,S,IT, TIn>::first(T &value) const {
	value = *head;
}

template<typename T, size_t S, typename IT, typename TIn>
T inline CircularBuffer<T,S,IT, TIn>::last() const {
	return *tail;
}

template<typename T, size_t S, typename IT, typename TIn>
void inline CircularBuffer<T,S,IT, TIn>::last(T &value) const {
	value = *tail;
}

template<typename T, size_t S, typename IT, typename TIn>
T CircularBuffer<T,S,IT, TIn>::operator [](IT index) const {
	if (index >= count) return *tail;
	return *(buffer + ((head - buffer + index) % capacity));
}

template<typename T, size_t S, typename IT, typename TIn>
IT inline CircularBuffer<T,S,IT, TIn>::size() const {
	return count;
}

template<typename T, size_t S, typename IT, typename TIn>
IT inline CircularBuffer<T,S,IT, TIn>::available() const {
	return capacity - count;
}

template<typename T, size_t S, typename IT, typename TIn>
bool inline CircularBuffer<T,S,IT, TIn>::isEmpty() const {
	return count == 0;
}

template<typename T, size_t S, typename IT, typename TIn>
bool inline CircularBuffer<T,S,IT, TIn>::isFull() const {
	return count == capacity;
}

template<typename T, size_t S, typename IT, typename TIn>
void inline CircularBuffer<T,S,IT, TIn>::clear() {
	head = tail = buffer;
	count = 0;
}

template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::copyToArray(T* dest) const {
    const T* finish = dest + count;
    for (const T* current = head; current < (buffer + capacity) && dest < finish; current++, dest++) {
        *dest = *current;
    }
    for (const T* current = buffer; current <= tail && dest < finish; current++, dest++) {
        *dest = *current;
    }
}

template<typename T, size_t S, typename IT>
template<typename R>
void inline CircularBuffer<T,S,IT>::copyToArray(R* dest, R (&convertFn)(const T&)) const {
    const R* finish = dest + count;
    for (const T* current = head; current < (buffer + capacity) && dest < finish; current++, dest++) {
        *dest = convertFn(*current);
    }
    for (const T* current = buffer; current <= tail && dest < finish; current++, dest++) {
        *dest = convertFn(*current);
    }
}

#ifdef CIRCULAR_BUFFER_DEBUG
#include <string.h>
template<typename T, size_t S, typename IT, typename TIn>
void inline CircularBuffer<T,S,IT, TIn>::debug(Print* out) {
	for (IT i = 0; i < capacity; i++) {
		int hex = (int)buffer + i;
		out->print("[");
		out->print(hex, HEX);
		out->print("] ");
		out->print(*(buffer + i));
		if (head == buffer + i) {
			out->print("<-head");
		} 
		if (tail == buffer + i) {
			out->print("<-tail");
		}
		out->println();
	}
}

template<typename T, size_t S, typename IT, typename TIn>
void inline CircularBuffer<T,S,IT, TIn>::debugFn(Print* out, void (*printFunction)(Print*, T)) {
	for (IT i = 0; i < capacity; i++) {
		int hex = (int)buffer + i;
		out->print("[");
		out->print(hex, HEX);
		out->print("] ");
		printFunction(out, *(buffer + i));
		if (head == buffer + i) {
			out->print("<-head");
		} 
		if (tail == buffer + i) {
			out->print("<-tail");
		}
		out->println();
	}
}
#endif
