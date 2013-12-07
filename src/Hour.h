#ifndef HOUR_H
#define HOUR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "Error.h"
#include "float.h"

namespace Shipping {
	class Hour : public Ordinal<Hour, double> {
	public:
		static double max(){ return DBL_MAX; }

    Hour(double num) : Ordinal<Hour, double>(num) {
        std::cerr << "Hour must be a positive number" << std::endl;
        if (num < 0) { throw new ValueOutOfBoundsException("Hour must be a positive number."); }
    }

		void operator+=(Hour other){
          value_ += other.value();
        }
	};
}
#endif
