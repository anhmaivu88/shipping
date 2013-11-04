#ifndef FLEET_H
#define FLEET_H

#include "Engine.h"
#include "MilesPerHour.h"
#include "Capacity.h"
#include "DollarsPerMile.h"

namespace Shipping {
  class Fleet : public Entity<Fleet> {
  public:
    typedef Fwk::Ptr<Fleet> Ptr;

    void capacityIs(Capacity capacity) { capacity_ = capacity; }
    void costIs(DollarsPerMile cost) { cost_ = cost; }
    void speedIs(MilesPerHour speed) { speed_ = speed; }

    Capacity capacity() { return capacity_; }
    DollarsPerMile cost() { return cost_; }
    MilesPerHour speed() { return speed_; }
    
    Fleet::Ptr fleetNew(EntityName name) {
      Ptr ptr = new Fleet(name);
      ptr->deleteRef();

      return ptr;
    }

  private:
    Fleet(EntityName name) : Entity<Fleet>(name), speed_(0), cost_(0), capacity_(0) {};
    
    MilesPerHour speed_;
    DollarsPerMile cost_;
    Capacity capacity_;
  };
}

#endif
