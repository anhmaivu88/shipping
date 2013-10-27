#ifndef LOCATION_H
#define LOCATION_H

#include "Engine.h"
#include "Ptr.h"

namespace Shipping {
  class Location : public Entity<Location> {
  public: 
    typedef Fwk::Ptr<Location> Ptr;
  };
}

#endif
