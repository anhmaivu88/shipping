#ifndef QUERY_H
#define QUERY_H

#include "Engine.h"
#include "Mile.h"
#include "DollarsPerMile.h"
#include "Hour.h"

namespace Shipping {

  class Query {
  public:
  	Query(): distance_(Mile::max()), cost_(Dollar::max()), time_(Hour::max()), priority_(Shipping::normal()){}
    
    void distanceIs(Mile distance){ distance_ = distance; }
    void costIs(Dollar cost){ cost_ = cost; }
    void timeIs(Hour tiem){ time_ = tiem; }
    void expeditedIs(Segment::Priority priority){ priority_ = priority; }

    Mile distance(){ return distance_; }
    Dollar cost(){ return cost_; }
    Hour time(){ return time_; }
    Segment::Priority priority(){ return priority_; }

  private:
  	Mile distance_;
    Dollar cost_;
    Hour time_;
    Segment::Priority priority_;

  };
}

#endif
