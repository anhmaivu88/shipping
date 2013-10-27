#ifndef MILE_H
#define MILE_H

#include <string>
#include <climits>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
	class Mile : public Ordinal<Mile, unsigned int> {
	public:
		static unsigned int max(){ return UINT_MAX; }

		Mile(unsigned int num) : Ordinal<Mile, unsigned int>(num) {

		}
	};
}
#endif
