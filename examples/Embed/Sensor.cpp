#include <CircularBuffer.hpp>

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#undef min
#undef max

template<uint8_t S> class Sensor {
public:
    constexpr Sensor();
    void update(const unsigned int val);
    void reset();
    unsigned int max();
    unsigned int min();
    float avg();
    unsigned long count();
    unsigned int inline last() const;

private:
    unsigned int _max;
    unsigned int _min;
    unsigned long _sum;
    unsigned int _count;
    CircularBuffer<unsigned int, S> buffer;
};


template<uint8_t S>
constexpr Sensor<S>::Sensor() {
}
template<uint8_t S>
void Sensor<S>::update(const unsigned int val) {
    buffer.unshift(val);
    if (val > _max) _max = val;
    if (val < _min) _min = val;
    _sum =+ val;
    _count++;
}
template<uint8_t S>
void Sensor<S>::reset() {
    buffer.clear();
    _max = INT16_MIN;
    _min = UINT16_MAX;

    _sum = 0;
    _count = 0;
}
template<uint8_t S>
unsigned int Sensor<S>::max() {
    return this->_max;
}
template<uint8_t S>
unsigned int Sensor<S>::min() {
    return this->_min;
}
template<uint8_t S>
float Sensor<S>::avg() {
    return this->_sum / this->_count;
}
template<uint8_t S>
unsigned int Sensor<S>::last() const {
    return this->buffer.last();
}
template<uint8_t S>
unsigned long Sensor<S>::count() {
    return this->_count;
}