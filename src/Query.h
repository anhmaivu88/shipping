#ifndef QUERY_H
#define QUERY_H

#include "Engine.h"
#include "Mile.h"
#include "DollarsPerMile.h"
#include "Dollar.h"
#include "Hour.h"
#include "Location.h"

namespace Shipping {

  class Query {
  public:
    
    enum Type {
      explore,
      connect
    };

  	Query(Type type): type_(type), distance_(Mile::max()), cost_(Dollar::max()), time_(Hour::max()), priority_(Segment::normal()){}

    // void typeIs(Type type){ type_ = type; }

    void startIs(Location::Ptr start){ start_ = start; }
    void endIs(Location::Ptr end){ end_ = end; }


    void distanceIs(Mile distance){ distance_ = distance; }
    void costIs(Dollar cost){ cost_ = cost; }
    void timeIs(Hour tiem){ time_ = tiem; }
    void expeditedIs(Segment::Priority priority){ priority_ = priority; }

    Type type(){ return type_; }

    Location::Ptr start(){ return start_; }
    Location::Ptr end(){ return end_; }

    Mile distance(){ return distance_; }
    Dollar cost(){ return cost_; }
    Hour time(){ return time_; }
    Segment::Priority priority(){ return priority_; }

  private:
    Type type_;

    Location::Ptr start_;
    Location::Ptr end_;

    Mile distance_;
    Dollar cost_;
    Hour time_;
    Segment::Priority priority_;
  };
}

#endif
