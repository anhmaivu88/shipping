#include "Location.h"
#include "Activity.h"

namespace Shipping {
  void Location::scheduleForwardingActivity(Shipment::Ptr shipment) {
    Activity::Ptr forwardingActivity = activityManager()->activityNew("....");
  }
}


