#ifndef PERCENTAGE_H
#define PERCENTAGE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
	class Percentage : public Ordinal<Percentage, double> {
	public:

		Percentage(double num) : Ordinal<Percentage, double>(num) {
      if (num < 0 || num > 1.0) { throw new ValueOutOfBoundsException("Percentages must be within the range 0.0-1.0"); }
		}

    virtual operator std::string() const { 
    std::stringstream stringRep;
    stringRep << std::setprecision(2) << std::fixed;
    stringRep << value() * 100;
    return stringRep.str();
  }
	};
}
#endif
