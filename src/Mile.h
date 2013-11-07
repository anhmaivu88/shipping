#ifndef MILE_H
#define MILE_H

#include <string>
#include <cfloat>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "Error.h"

namespace Shipping {
	class Mile : public Ordinal<Mile, double> {
	public:
		static double max(){ return DBL_MAX; }

		Mile(double num) : Ordinal<Mile, double>(num) {
      if (num < 0) { throw new ValueOutOfBoundsException("Miles must be positive."); }
		}

    operator std::string() const { return std::to_string(value()); }
	};
}
#endif
