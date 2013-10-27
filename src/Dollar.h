#ifndef DOLLAR_H
#define DOLLAR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
	class Dollar : public Ordinal<Dollar, unsigned int> {
	public:
		static unsigned int max(){ return UINT_MAX; }

		Dollar(unsigned int num) : Ordinal<Dollar, unsigned int>(num) {

		}
	};
}
#endif
