#ifndef HOUR_H
#define HOUR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"

namespace Shipping {
	class Hour : public Ordinal<Hour, double> {
	public:
		static double max(){ return DBL_MAX; }

  Hour(double num) : Ordinal<Hour, double>(num) {}

		void operator+=(Hour other){
      value_ += other.value();
    }
	};
}
#endif
