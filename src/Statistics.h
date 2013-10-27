#ifndef STATISTICS_H
#define STATISTICS_H

#include "Engine.h"
#include <map>
#include "Segment.h"
#include "Location.h"

using namespace std;

namespace Shipping {
  class Statistics : Entity<Statistics> {
  public:
    typedef Fwk::Ptr<Statistics> Ptr;
    typedef float Percentage;

    EntityCount locationType(Location::Type type) { return locationStats_[type]; }
    EntityCount segmentType(Segment::Type type) { return segmentStats_[type]; }

    Percentage expeditedShippingPercentage();

  private:
    Statistics(EntityName name) : Entity(name) {}

    map<Location::Type, int> locationStats_;
    map<Segment::Type, int> segmentStats_;
  };
}

#endif
