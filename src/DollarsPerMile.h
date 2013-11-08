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

        }

        void operator*=(double scalar){
            if (scalar < 0) { throw new ValueOutOfBoundsException("Miles per hour must be positive."); }
            value_ *= scalar;
        }

      operator std::string() const { return std::to_string(value()); }
    };
}
#endif
