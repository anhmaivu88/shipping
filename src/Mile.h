#ifndef MILE_H
#define MILE_H

#include <string>
#include <cfloat>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "Error.h"
#include "DollarsPerMile.h"
#include "Dollar.h"
#include "Hour.h"
#include "MilesPerHour.h"

namespace Shipping {
    class Mile : public Ordinal<Mile, double> {
    public:
        static double max(){ return DBL_MAX; }

        Mile(double num) : Ordinal<Mile, double>(num) {
            if (num < 0) { 
                std::cerr << "Miles must be positive" << std::endl;
                throw new ValueOutOfBoundsException("Miles must be positive."); 
            }
        }

        Dollar operator*(DollarsPerMile dpm){
            return Dollar(value_ * dpm.value());
        }

        Hour operator/(MilesPerHour mph){
            return Hour(value_ / mph.value());
        }

        void operator+=(Mile other){
            value_ += other.value();
        }
    };
}
#endif
