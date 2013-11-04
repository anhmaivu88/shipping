#ifndef MILESPERHOUR_H
#define MILESPERHOUR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"
#include "Error.h"

namespace Shipping {
	class MilesPerHour : public Ordinal<MilesPerHour, double> {
	public:

		MilesPerHour(double num) : Ordinal<MilesPerHour, double>(num) {
			if(num < 0) throw new ValueOutOfBoundsException("expected nonnegative value");
		}
	};
}
#endif
