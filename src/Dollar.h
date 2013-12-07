#ifndef DOLLAR_H
#define DOLLAR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"

namespace Shipping {
	class Dollar : public Ordinal<Dollar, double> {
	public:
		static double max(){ return DBL_MAX; }

		Dollar(double num) : Ordinal<Dollar, double>(num) {
            if (num < 0) { 
                std::cerr << "Dollar must be a positive integer" << std::endl;
                throw new ValueOutOfBoundsException("Dollar must be a positive integer."); 
            }
		}

		void operator+=(Dollar other){
      value_ += other.value();
    }
	};
}
#endif
