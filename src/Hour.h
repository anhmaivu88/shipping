#ifndef HOUR_H
#define HOUR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
	class Hour : public Ordinal<Hour, unsigned int> {
	public:
		static unsigned int max(){ return UINT_MAX; }

		Hour(unsigned int num) : Ordinal<Hour, unsigned int>(num) {

		}
	};
}
#endif
