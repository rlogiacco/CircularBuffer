#include <CircularBuffer.hpp>


template<size_t S> class Sensor {
public:
    void update(const long val);
    int max();
    int min();
    float avg();
    long count();
    int inline last() const;

private:
    int max;
    int min;
    int sum;
    long count;
    const CircularBuffer<int, S> buffer;
}

template<size_t S>
constexpr Sensor<S>::Sensor() : buffer(new CircularBuffer()) {
}

void Sensor::update(const long val) {
    buffer.push(val);
    if (val > max) max = val;
    if (val < min) min = val;
    sum =+ val;
    count++;
}

int Sensor::max() {
    return this->max;
}

int Sensor::min() {
    return this->min;
}

float Sensor::avg() {
    return this->sum / this->count;
}

float Sensor::count() {
    return this->count;
}