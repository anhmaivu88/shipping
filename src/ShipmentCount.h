#ifndef SHIPMENTCOUNT_H
#define SHIPMENTCOUNT_H

#include <string>
#include <cfloat>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "Error.h"

namespace Shipping {
    class ShipmentCount : public Ordinal<ShipmentCount, double> {
    public:
        static double max(){ return DBL_MAX; }

        ShipmentCount(double num) : Ordinal<ShipmentCount, double>(num) {
            if (num < 0) { 
                std::cerr << "ShipmentCount must be positive" << std::endl;
                throw new ValueOutOfBoundsException("ShipmentCounts must be positive."); 
            }
        }

        void operator+=(ShipmentCount other){
            value_ += other.value();
        }
    };
}
#endif
