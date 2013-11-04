#ifndef STATISTICS_H
#define STATISTICS_H

#include "Engine.h"
#include <map>
#include "Segment.h"
#include "Location.h"

using namespace std;

namespace Shipping {
  class Statistics : public Entity<Statistics> {
  public:
    typedef Fwk::Ptr<Statistics> Ptr;
    typedef float Percentage;

    EntityCount locationType(Location::Type type) { return locationStats_[type]; }
    EntityCount segmentType(Segment::Type type) { return segmentStats_[type]; }

    Percentage expeditedShippingPercentage();

    static Statistics::Ptr statisticsNew(EntityName name, Engine::Ptr engine) {
      Ptr ptr = new Statistics(name, engine);
      ptr->deleteRef();

      return ptr;
    }

  private:
    Engine::Ptr engine_;

    map<Location::Type, int> locationStats_;
    map<Segment::Type, int> segmentStats_;

    Statistics(EntityName name, Engine::Ptr engine) : Entity(name), engine_(engine) {}
  };
}

#endif
