#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Entity.h"
#include "Location.h"
#include "Statistics.h"
#include "Connectivity.h"
#include "Segment.h"
#include "Fleet.h"
#include "Error.h"

namespace Shipping {
  class Engine : public Entity<Engine> {
    typedef Fwk::Ptr<Engine> Ptr;
  };
} /* end namespace */

#endif
