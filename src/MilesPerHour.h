#ifndef MILESPERHOUR_H
#define MILESPERHOUR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
    class MilesPerHour : public Ordinal<MilesPerHour, double> {
    public:

        MilesPerHour(double num) : Ordinal<MilesPerHour, double>(num) {
            if (num < 0) { throw new ValueOutOfBoundsException("Miles per hour must be positive."); }
        }

        void operator*=(double scalar){
            if (scalar < 0) { throw new ValueOutOfBoundsException("Miles per hour must be positive."); }
            value_ *= scalar;
        }

        operator std::string() const { return std::to_string(value()); } 
    };
}
#endif
