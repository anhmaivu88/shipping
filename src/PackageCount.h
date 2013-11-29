#ifndef PACKAGECOUNT_H
#define PACKAGECOUNT_H

#include <string>
#include <cfloat>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "Error.h"

namespace Shipping {
    class PackageCount : public Ordinal<PackageCount, double> {
    public:
        static double max(){ return DBL_MAX; }

        PackageCount(double num) : Ordinal<PackageCount, double>(num) {
            if (num < 0) { throw new ValueOutOfBoundsException("PackageCounts must be positive."); }
        }

        void operator+=(PackageCount other){
            value_ += other.value();
        }
    };
}
#endif
