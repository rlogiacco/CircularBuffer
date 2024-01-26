/*
 CircularBuffer.hpp - Circular buffer library for Arduino.
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
#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include <stdint.h>
#include <stddef.h>

#ifdef CIRCULAR_BUFFER_DEBUG
#include <Print.h>
#endif

namespace Helper {
	/** @private */
	template<bool FITS8, bool FITS16> struct Index {
		using Type = uint32_t;
	};

	/** @private */
	template<> struct Index<false, true> {
		using Type = uint16_t;
	};

	/** @private */
	template<> struct Index<true, true> {
		using Type = uint8_t;
	};
}

/**
 * @brief Implements a circular buffer that supports LIFO and FIFO operations.
 *
 * @tparam T The type of the data to store in the buffer.
 * @tparam S The maximum number of elements that can be stored in the buffer.
 * @tparam IT The data type of the index. Typically should be left as default.
 */
template<typename T, size_t S, typename IT = typename Helper::Index<(S <= UINT8_MAX), (S <= UINT16_MAX)>::Type> class CircularBuffer {
public:
	/**
	 * @brief The buffer capacity.
	 *
	 * Read only as it cannot ever change.
	 */
	static constexpr IT capacity = static_cast<IT>(S);

	/**
	 * @brief Aliases the index type.
	 *
	 * Can be used to obtain the right index type with `decltype(buffer)::index_t`.
	 */
	using index_t = IT;

	/**
	 * @brief Create an empty circular buffer.
	 */
	constexpr CircularBuffer();

	// disable the copy constructor
	/** @private */
	CircularBuffer(const CircularBuffer&) = delete;
	/** @private */
	CircularBuffer(CircularBuffer&&) = delete;

	// disable the assignment operator
	/** @private */
	CircularBuffer& operator=(const CircularBuffer&) = delete;
	/** @private */
	CircularBuffer& operator=(CircularBuffer&&) = delete;

	/**
	 * @brief Adds an element to the beginning of buffer.
	 *
	 * @return `false` iff the addition caused overwriting to an existing element.
	 */
	bool unshift(T value);

	/**
	 * @brief Adds an element to the end of buffer.
	 *
	 * @return `false` iff the addition caused overwriting to an existing element.
	 */
	bool push(T value);

	/**
	 * @brief Removes an element from the beginning of the buffer.
	 *
	 * @warning Calling this operation on an empty buffer has an unpredictable behaviour.
	 */
	T shift();

	/**
	 * @brief Removes an element from the end of the buffer.
	 *
	 * @warning Calling this operation on an empty buffer has an unpredictable behaviour.
	 */
	T pop();

	/**
	 * @brief Returns the element at the beginning of the buffer.
	 *
	 * @return The element at the beginning of the buffer.
	 */
	T inline first() const;

	/**
	 * @brief Returns the element at the end of the buffer.
	 *
	 * @return The element at the end of the buffer.
	 */
	T inline last() const;

	/**
	 * @brief Array-like access to buffer.
	 *
	 * Calling this operation using and index value greater than `size - 1` returns the tail element.
	 *
	 * @warning Calling this operation on an empty buffer has an unpredictable behaviour.
	 */
	T operator [] (IT index) const;

	/**
	 * @brief Returns how many elements are actually stored in the buffer.
	 *
	 * @return The number of elements stored in the buffer.
	 */
	IT inline size() const;

	/**
	 * @brief Returns how many elements can be safely pushed into the buffer.
	 *
	 * @return The number of elements that can be safely pushed into the buffer.
	 */
	IT inline available() const;

	/**
	 * @brief Check if the buffer is empty.
	 *
	 * @return `true` iff no elements can be removed from the buffer.
	 */
	bool inline isEmpty() const;

	/**
	 * @brief Check if the buffer is full.
	 *
	 * @return `true` if no elements can be added to the buffer without overwriting existing elements.
	 */
	bool inline isFull() const;

	/**
	 * @brief Resets the buffer to a clean status, making all buffer positions available.
	 *
	 * @note This does not clean up any dynamically allocated memory stored in the buffer.
	 * Clearing a buffer that points to heap-allocated memory may cause a memory leak, if it's not properly cleaned up.
	 */
	void inline clear();


    /**
	 * @brief Copies the buffer content into the provided array.
	 *
	 * @note No verification is done about the provided array length, it's the user responsibility to ensure the array provides enough space to accomodate
	 * all the elements currently stored in the buffer. After the function returns the elements in the buffer can be found starting at index 0 and up to the buffer size() at the moment of the copyToArray function call.
	 */
	void copyToArray(T* out) const;
    
    /**
	 * @brief Copies the buffer content into the provided array calling the provided conversion function for each and every element of the buffer.
	 *
	 * @note No verification is done about the provided array length, it's the user responsibility to ensure the array provides enough space to accomodate
	 * all the elements currently stored in the buffer. After the function returns the elements in the buffer can be found starting at index 0 and up to the buffer size() at the moment of the copyToArray function call.
	 * 
	 * @param convertFn the conversion function to call for each item stored in the buffer
	 */
	template<typename R>
    void copyToArray(R* out, R (&convertFn)(const T&)) const;

	#ifdef CIRCULAR_BUFFER_DEBUG
	void inline debug(Print* out);
	void inline debugFn(Print* out, void (*printFunction)(Print*, T));
	#endif

private:
	T buffer[S];
	T *head;
	T *tail;
#ifndef CIRCULAR_BUFFER_INT_SAFE
	IT count;
#else
	volatile IT count;
#endif
};

#include <CircularBuffer.tpp>
#endif
