#include "PtrInterface.h"
#include "Location.h"
#include "Segment.h"
#include "PackageCount.h"

namespace Shipping {
  class Shipment : public Fwk::PtrInterface<Shipment> {
  public:
    typedef Fwk::Ptr<Shipment> Ptr;

    class Notifiee : public Fwk::PtrInterface<Notifiee> {
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onArrive(Location::Ptr arrivalLocation) {}
    };
  private:
    Location::Ptr source_;
    Location::Ptr destination_;
    Segment::Ptr currentSegment_;
    Hour transitTime_;
    Dollar shippingCost_;
    PackageCount packageCount_;
  };
}
