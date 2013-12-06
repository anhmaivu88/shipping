#include "Segment.h"
#include "Error.h"
#include <iostream>

namespace Shipping {
  void Segment::priorityIs(Priority priority) {
    if (priority != priority_) {
      priority_ = priority; 
      for (auto notifiee : notifiees_) { try { notifiee->onPriority(priority); } catch (...) { } }
    }
  }

  void Segment::returnSegmentIs(Segment::Ptr returnSegment) {
    if (returnSegment_ != returnSegment) {
      if (returnSegment == NULL) {
        returnSegment_ = NULL;
        return;
      }

      if (returnSegment->type() != type())
        throw new ValueOutOfBoundsException("Attempted to set mismatched return segment.");
      
      if (returnSegment_)
        returnSegment_->returnSegmentIs(NULL);

      returnSegment_ = returnSegment;
      // Will not infinite loop because of idempotency.
      returnSegment->returnSegmentIs(this);
    }
  }

  void Segment::sourceIs(LocationPtr location) {
    if (location == (LocationPtr) NULL) {
      source_->segmentDel(Ptr(this));
      source_ = (LocationPtr) NULL;
      returnSegmentIs((Segment::Ptr) NULL);
    }

    if (source_ != location) {
      if (location) { location->segmentAdd(Ptr(this)); }
      if (source_) { source_->segmentDel(Ptr(this));  }
      source_ = location;
    }
  }

  void Segment::shipmentAdd(Shipment::Ptr shipment) {
    shipments_.push_back(shipment);

    for (auto notifiee : notifiees_) {
      notifiee->onShipmentAdd(shipment);
    }
  }

  void Segment::shipmentDel(Shipment::Ptr shipment) {
    shipments_.erase(find(shipments_.begin(), shipments_.end(), shipment));

    for (auto notifiee : notifiees_) {
      notifiee->onShipmentDel(shipment);
    }
  }
}
