#ifndef TRANSFERACTIVITYREACTOR_H
#define TRANSFERACTIVITYREACTOR_H
#include "Segment.h"
#include "Location.h"
#include "Shipment.h"
#include "Activity.h"

/** Need to create a reactor on segments if we can't route anything. */

namespace Shipping {
  class TransferActivityReactor : public Activity::Notifiee {
  public: 
    void onStatus() {
      /* If we're executing then it means our shipment has arrived. */
      if (notifier()->status() == Activity::Status::executing) {
        notifier()->statusIs(Activity::Status::deleted);
        segment()->shipmentDel(shipment());
        segment()->returnSegment()->source()->shipmentAdd(shipment());
      }
    }

    TransferActivityReactor(Segment::Ptr segment, Shipment::Ptr shipment, Activity *activity) : Notifiee(activity), segment_(segment), shipment_(shipment) {}

  private:
    Segment::Ptr segment() { return segment_; }
    Shipment::Ptr shipment() { return shipment_; }

    Segment::Ptr segment_;
    Shipment::Ptr shipment_;
  };
}

#endif
