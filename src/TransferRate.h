#ifndef TRANSFERRATE_H
#define TRANSFERRATE_H

#include <string>
#include <cfloat>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "Error.h"

namespace Shipping {
    class TransferRate : public Ordinal<TransferRate, double> {
    public:
        static double max(){ return DBL_MAX; }

        TransferRate(double num) : Ordinal<TransferRate, double>(num) {
            if (num < 0) { throw new ValueOutOfBoundsException("TransferRates must be positive."); }
        }

        void operator+=(TransferRate other){
            value_ += other.value();
        }
    };
}
#endif
