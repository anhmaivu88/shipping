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
    
    static Fleet::Ptr fleetNew(EntityName name) {
      Ptr ptr = new Fleet(name);

      return ptr;
    }

    static std::string boat(){ return "Boat"; }
    static std::string truck(){ return "Truck"; }
    static std::string plane(){ return "Plane"; }

  private:
    Fleet(EntityName name) : Entity<Fleet>(name), speed_(1), cost_(1), capacity_(1) {};
    
    MilesPerHour speed_;
    DollarsPerMile cost_;
    Capacity capacity_;
  };
}

#endif
