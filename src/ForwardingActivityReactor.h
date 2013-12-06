#ifndef FORWARDINGACTIVITYREACTOR_H
#define FORWARDINGACTIVITYREACTOR_H
#include "Segment.h"
#include "Location.h"
#include "Shipment.h"
#include "Activity.h"
#include "Error.h"
#include <iostream>

/** Need to create a reactor on segments if we can't route anything. */

namespace Shipping {
  class ForwardingActivityReactor : public Activity::Notifiee {
  public: 
    static ForwardingActivityReactor::Ptr forwardingActivityReactorNew(Location::Ptr location, Activity *activity) {
      return Ptr(new ForwardingActivityReactor(location, activity));
    }

    void onStatus() {
      /* We attempt to forward a shipment if we are scheduled. We will forward at most 
         one per execution, although we will potentially check all of the shipments. */
      if (notifier()->status() == Activity::Status::executing) {
        std::cout << "Attempting to route packages on " << location()->name() << std::endl;
        bool foundForwardablePackage = false;
        for (int i = 0; i < location()->shipmentCount(); i++) {
          Shipment::Ptr shipment = location()->shipment(i);
          Segment::Ptr nextSegment = shipment->path().nextSegment(location());

          if (nextSegment->shipmentCount() < nextSegment->capacity()) {
            std::cout << "nextSegment's capacity is: " << nextSegment->capacity().value() << " and its count is " << nextSegment->shipmentCount().value() << std::endl;
            nextSegment->shipmentAdd(shipment);
            location()->shipmentDel(shipment);
            foundForwardablePackage = true;
          } else {
            /* If the segment we want to route on is full, then we create a reactor
               on the segment that will wake us up when it has free capacity. */
            std::cout << "capacity was full so we wait a bit." << std::endl;
          }
        }

        /* If we're out of shipments, or they're all waiting on capacity to free up, we want to wait. */
        if (location()->shipmentCount() > 0 && foundForwardablePackage) {
          this->notifier()->statusIs(Activity::Status::ready);
        } else {
          notifier()->nextTimeIs(notifier()->nextTime() + Hour(100));
          notifier()->statusIs(Activity::Status::ready);
        }
      }
    }

    ForwardingActivityReactor(Location::Ptr location, Activity *activity) : Notifiee(activity), location_(location) {}

  private:
    Location::Ptr location() { return location_; }
    Location::Ptr location_;

    class SegmentCapacityReactor : public Segment::Notifiee {
    public:
      void onShipmentDel(Shipment::Ptr shipment) {
        forwardingActivity()->statusIs(Activity::Status::ready);
        notifierIs(NULL);
      }

      SegmentCapacityReactor(Activity::Ptr forwardingActivity, Segment *segment) : Notifiee(segment), forwardingActivity_(forwardingActivity) {
      }

    private:
      Activity::Ptr forwardingActivity() { return forwardingActivity_; }
      Activity::Ptr forwardingActivity_;
    };
    
    map<EntityName, SegmentCapacityReactor::Ptr> capacityReactors_;
  };
}

#endif
