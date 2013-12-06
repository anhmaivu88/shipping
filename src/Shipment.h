#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "PtrInterface.h"
#include "Location.h"
#include "Segment.h"
#include "PackageCount.h"
#include "Notifiee.h"
#include "PathData.h"

namespace Shipping {
  class Shipment : public Fwk::PtrInterface<Shipment> {
  public:
    typedef Fwk::Ptr<Shipment> Ptr;

    class Notifiee : public Fwk::BaseNotifiee<Shipment> {
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onArrive(LocationPtr arrivalLocation) {}
    };

    void notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

    LocationPtr source() { return source_; }
    LocationPtr destination() { return destination_; }
    PathData path() { return path_; }
    
    Hour transitTime() { return transitTime_; }
    Dollar shippingCost() { return shippingCost_; }
    PackageCount packageCount() { return packageCount_; }

    ~Shipment() {
      for (auto notifiee : notifiees_) {
        notifiee->notifierIs(NULL);
      }
    }

  private:
    typedef Fwk::Ptr<Location> LocationPtr;
    typedef Fwk::Ptr<Segment> SegmentPtr;

    LocationPtr source_;
    LocationPtr destination_;
    SegmentPtr currentSegment_;
    Hour transitTime_;
    Dollar shippingCost_;
    PackageCount packageCount_;
    std::vector<Notifiee *> notifiees_;
    PathData path_;
  };
}

#endif
