#ifndef CAPACITY_H
#define CAPACITY_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"

namespace Shipping {
  class Capacity : public Ordinal<Capacity, int> {
  public:

  Capacity(int num) : Ordinal<Capacity, int>(num) {
    if (num < 0) { throw new ValueOutOfBoundsException("Capacity must be a positive integer."); }
  }
  };
}
#endif
