#ifndef MICROSECOND_H
#define MICROSECOND_H
#include "Nominal.h"
#include "ActivityTime.h"

class Microsecond : public Ordinal<Microsecond,double> {
public:
    Microsecond(double time) : Ordinal<Microsecond,double>(time)
    {}

    Time operator/(Time other){
        return Time(value_ / other.value());
    }
};

#endif
