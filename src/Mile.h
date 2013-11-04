#ifndef MILE_H
#define MILE_H

#include <string>
#include <cfloat>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
	class Mile : public Ordinal<Mile, double> {
	public:
		static double max(){ return DBL_MAX; }

		Mile(double num) : Ordinal<Mile, double>(num) {

		}
	};
}
#endif
