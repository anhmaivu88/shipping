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
#include "Query.h"
#include <map>
#include <vector>

namespace Shipping {


  class Engine : public Entity<Engine> {
  public:
    typedef Fwk::Ptr<Engine> Ptr;

    Segment::Ptr planeSegmentNew(EntityName name);
    Segment::Ptr boatSegmentNew(EntityName name);
    Segment::Ptr truckSegmentNew(EntityName name);
    Segment::Ptr segmentNew(EntityName name, Segment::Type type);
    Segment::Ptr segment(EntityName name);

    Location::Ptr customerLocationnew(EntityName name);
    Location::Ptr portLocationNew(EntityName name);
    Location::Ptr terminalLocationNew(EntityName name);
    Location::Ptr locationNew(EntityName name, Location::Type type);
    Location::Ptr location(EntityName name);

    Fleet::Ptr fleet(EntityName name);

    Statistics::Ptr statistics();

    Query queryNew();

  private:
    map<EntityName, Segment::Ptr> segments_;
    map<EntityName, Location::Ptr> locations_;
    map<EntityName, Fleet::Ptr> fleets_;

    Statistics::Ptr statistics_;
  };
} /* end namespace */

#endif
