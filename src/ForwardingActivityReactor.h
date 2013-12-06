#include "Segment.h"
#include "Shipment.h"
#include "Activity.h"

/** Need to create a reactor on segments if we can't route anything. */

class ForwardingActivityReactor : public Activity::Notifiee {
public: 
  void onStatus() {
    if (notifier()->status() == Activity::Status::executing) {
      for (int i = 0; i < location()->shipmentCount(); i++) {
        Shipment::Ptr shipment = location()->shipment(i);
      }
    }
  }

private:
  Location::Ptr location() { return location_; }
  Location::Ptr location_;
};
