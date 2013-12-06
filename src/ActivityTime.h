#ifndef ACTIVITYTIME_H
#define ACTIVITYTIME_H
#include "Nominal.h"

class Time : public Ordinal<Time,double> {
public:
    Time(double time) : Ordinal<Time,double>(time)
    {}

    Time operator+(Time other){
        return Time(value_ + other.value());
    }
};

#endif
