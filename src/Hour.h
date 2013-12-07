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
      if (num < 0) { 
        std::cerr << "Hour must be a positive number" << std::endl;
        throw new ValueOutOfBoundsException("Hour must be a positive number."); 
      }
    }

		void operator+=(Hour other){
      value_ += other.value();
    }

    Hour operator-(const Hour &other) {
      return Hour(value() - other.value());
    }

    void operator/=(int other) {
      value_ /= other;
    }
	};
}
#endif
