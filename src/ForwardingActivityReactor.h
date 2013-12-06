#ifndef FORWARDINGACTIVITYREACTOR_H
#define FORWARDINGACTIVITYREACTOR_H
#include "Segment.h"
#include "Location.h"
#include "Shipment.h"
#include "Activity.h"
#include "Error.h"

/** Need to create a reactor on segments if we can't route anything. */

namespace Shipping {
  class ForwardingActivityReactor : public Activity::Notifiee {
  public: 

    void onStatus() {
      /* We attempt to forward a shipment if we are scheduled. We will forward at most 
         one per execution, although we will potentially check all of the shipments. */
      if (notifier()->status() == Activity::Status::executing) {
        bool foundForwardablePackage = false;
        for (int i = 0; i < location()->shipmentCount(); i++) {
          Shipment::Ptr shipment = location()->shipment(i);
          Segment::Ptr nextSegment = shipment->path().nextSegment(location());

          if (nextSegment == NULL) {
            /* FIXME: we need to handle this properly. */
            // throw new UnroutableShipmentException("Unable to find route for shipment.");
          }

          if (nextSegment->shipmentCount() < nextSegment->capacity()) {
            nextSegment->shipmentAdd(shipment);
            location()->shipmentDel(shipment);
            foundForwardablePackage = true;
          } else {
            /* If the segment we want to route on is full, then we create a reactor
               on the segment that will wake us up when it has free capacity. */
            new SegmentCapacityReactor(notifier(), nextSegment.ptr());
          }
        }

        /* If we're out of shipments, or they're all waiting on capacity to free up, we want to wait. */
        if (location()->shipmentCount() > 0 && foundForwardablePackage) {
          this->notifier()->statusIs(Activity::Status::ready);
        } else {
          this->notifier()->statusIs(Activity::Status::waiting);
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
        notifier()->notifieeDel(this);
      }

      SegmentCapacityReactor(Activity::Ptr forwardingActivity, Segment *segment) : Notifiee(segment), forwardingActivity_(forwardingActivity) {
        notifier()->notifieeAdd(this);
      }

    private:
      Activity::Ptr forwardingActivity() { return forwardingActivity_; }
      Activity::Ptr forwardingActivity_;
    };
  };
}

#endif
