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
    if (source_ != location) {
      if (source_) {
        source_->segmentDel(Ptr(this));
      }
      source_ = location;
      location->segmentAdd(Ptr(this));
    }
  }
}
