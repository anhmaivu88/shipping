#ifndef ACTIVITYTIME_H
#define ACTIVITYTIME_H
#include "Nominal.h"

class Time : public Ordinal<Time,double> {
public:
    Time(double time) : Ordinal<Time,double>(time)
    {}
};

#endif
