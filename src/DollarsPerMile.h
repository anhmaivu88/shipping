#ifndef DOLLARSPERMILE_H
#define DOLLARSPERMILE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"

namespace Shipping {
    class DollarsPerMile : public Ordinal<DollarsPerMile, double> {
    public:

        DollarsPerMile(double num) : Ordinal<DollarsPerMile, double>(num) {
            if (num < 0) { 
                std::cerr << "Capacity must be a positive integer" << std::endl;
                throw new ValueOutOfBoundsException("Capacity must be a positive integer."); 
            }
        }

        void operator*=(double scalar){
            if (scalar < 0) { 
                std::cerr << "DollarsPerMile must be a positive integer" << std::endl;
                throw new ValueOutOfBoundsException("Dollars per mile must be positive."); 
            }
            value_ *= scalar;
        }
    };
}
#endif
