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


# ifndef NAN
#  define NAN (__builtin_nanf(""))
# endif

template<typename T, size_t S, typename IT>
constexpr CircularBuffer<T,S,IT>::CircularBuffer() :
		head(buffer), tail(buffer), count(0) {
}

template<typename T, size_t S, typename IT> 
bool CircularBuffer<T,S,IT>::unshift(T value) {
	if (isnan((float32_t)value))		errorFlag = true;
	insertedCnt++;
	if (head == buffer) {
		head = buffer + capacity;
	}
	T oldval = *head;
	*--head = value;
	if (count == capacity) {
		if (tail-- == buffer) {
			tail = buffer + capacity - 1;
		}
		toRemove(oldval);
		wasAdded(value);
		return false;
	} else {
		if (count++ == 0) {
			tail = head;
		}
		wasAdded(value);
		return true;
	}
	return true;
}

template<typename T, size_t S, typename IT> 
bool CircularBuffer<T,S,IT>::push(T value) {
	if (isnan((float32_t)value))		errorFlag = true;
	insertedCnt++;
	if (++tail == buffer + capacity) {
		tail = buffer;
	}
	T oldval = *tail;
	*tail = value;
	if (count == capacity) {
		if (++head == buffer + capacity) {
			head = buffer;
		}
		toRemove(oldval);
		wasAdded(value);
		return false;
	} else {
		if (count++ == 0) {
			head = tail;
		}
		wasAdded(value);
		return true;
	}
	return true;
}

template<typename T, size_t S, typename IT> 
T CircularBuffer<T,S,IT>::shift() {
	// set an error condition if the buffer is empty
	if (count <= 0) {
		errorFlag = true;
		return 0;
	}
	insertedCnt--;
	toRemove(*head);
	T result = *head++;
	if (head >= buffer + capacity) {
		head = buffer;
	}
	count--;
	if (isnan((float32_t)result))		errorFlag = true;
	return result;
}

template<typename T, size_t S, typename IT> 
T CircularBuffer<T,S,IT>::pop() {
	// set an error condition if the buffer is empty
	if (count <= 0) {
		errorFlag = true;
		return 0;
	}
	insertedCnt--;
	toRemove(*tail);
	T result = *tail--;
	if (tail < buffer) {
		tail = buffer + capacity - 1;
	}
	count--;
	if (isnan(result))		errorFlag = true;
	return result;
}

template<typename T, size_t S, typename IT> 
T inline CircularBuffer<T,S,IT>::first() const {
	return *head;
}

template<typename T, size_t S, typename IT> 
T inline CircularBuffer<T,S,IT>::last() const {
	return *tail;
}

template<typename T, size_t S, typename IT> 
T CircularBuffer<T,S,IT>::operator [](IT index) const {
	return *(buffer + ((head - buffer + index) % capacity));
}

template<typename T, size_t S, typename IT> 
IT inline CircularBuffer<T,S,IT>::size() const {
	return count;
}

template<typename T, size_t S, typename IT> 
IT inline CircularBuffer<T,S,IT>::available() const {
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
void CircularBuffer<T,S,IT>::clear() {
	head = tail = buffer;
	count = 0;
	_msRun = 0.0;
	_mean = 0.0;
	_sum = 0.0;
	errorFlag = false;
}

template<typename T, size_t S, typename IT>
void CircularBuffer<T,S,IT>::wasAdded(T val) {
	_sum = _sum + val;
    float64_t nextM = _mean + ((val - _mean) / this->size());
	if (isnan((float64_t)nextM)) errorFlag = true;
    _msRun = _msRun + ((val - _mean) * (val - nextM));
    _mean = nextM;
	if (isnan(_msRun)) errorFlag = true;
	if (_msRun < 0) errorFlag = true;
}

template<typename T, size_t S, typename IT>
void  CircularBuffer<T,S,IT>::toRemove(T val) {
	if (size() == 0) {
        //can't go negative size, throw exception
        _sum = 0.0; _mean = 0.0; _msRun = 0.0;
    } else if (size() == 1) { 
        _sum = 0.0; _mean = 0.0; _msRun = 0.0;
    } else {
		_sum = _sum - val;
        float64_t mMOld = (size() * _mean - val)/(size()-1);
		if (isnan(mMOld)) errorFlag = true;
        _msRun = _msRun - ((val - _mean) * (val - mMOld));
        _mean = mMOld;
		if (isnan(_msRun))		errorFlag = true;
		if (_msRun < 0) 		errorFlag = true;
    }
}

template<typename T, size_t S, typename IT>
float64_t inline CircularBuffer<T,S,IT>::mean() {
	return _mean;
}

template<typename T, size_t S, typename IT>
float64_t CircularBuffer<T,S,IT>::average() {
	return _sum / this->size();
}

template<typename T, size_t S, typename IT>
float64_t CircularBuffer<T,S,IT>::variance() {
	float64_t ret = 0.0;
	if (this->size() > 1) {
		ret = _msRun / (this->size()-1);
	}
	if (isnan(ret) || ret < 0)		errorFlag = true;
	return ret;
}

template<typename T, size_t S, typename IT>
float64_t CircularBuffer<T,S,IT>::cbstderr() {
	float64_t ret = 0.0;
	if (this->size() > 1) {
		ret = stdev() / sqrt(this->size());
	}
	if (isnan(ret) || ret < 0)		errorFlag = true;
	return ret;
}

template<typename T, size_t S, typename IT>
float64_t CircularBuffer<T,S,IT>::stdev() {
	float64_t ret = 0.0;
    if (this->size() < 2) return NAN; // otherwise DIV0 error
    ret = sqrt( this->variance() );
	if (isnan(ret) || ret < 0)		errorFlag = true;
	return ret;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::minimum(T tmin) {
  T minVal = maximum();
  for (int i = 1; i < this->size();i++) {
    if (
		((*this)[i] < minVal)
		&&
		((*this)[i] > tmin)
		) {
      minVal = (*this)[i];
    }
  }
  return minVal;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::minimum() {
  T minVal = (*this)[0];
  for (int i = 1; i < this->size();i++) {
    if (
		(*this)[i] < minVal
		) {
      minVal = (*this)[i];
    }
  }
  return minVal;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::maximum() { 
  T maxVal = (*this)[0];
  for (int i = 1; i < this->size();i++) {
    if ((*this)[i] > maxVal) {
      maxVal = (*this)[i];
    }
  }
  return maxVal;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::rank(uint8_t r) {
  if (r > this->size())
	return 0;

  T trank = minimum();
  for (int i=0; i<r; i++){
  	  T tmin = maximum();
	  for (int j=0; j < this->size(); j++) {
	    if ((*this)[j] < tmin
			&&
			(*this)[j] > trank
			) {
      		tmin = (*this)[j];
    	}
	  }
	  trank = tmin;
  }
  return trank;
}

#ifdef CIRCULAR_BUFFER_DEBUG
#include <string.h>
template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::debug(Print* out) {
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

template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::debugFn(Print* out, void (*printFunction)(Print*, T)) {
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

