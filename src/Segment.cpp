#include "Segment.h"

namespace Shipping {
  void Segment::priorityIs(Priority priority) {
    if (priority != priority_) {
      priority_ = priority; 
      for (auto notifiee : notifiees_) { notifiee->onPriority(priority); }
    }
  }

  void Segment::returnSegmentIs(Segment::Ptr returnSegment) {
    if (returnSegment_ != returnSegment) {
      returnSegment_ = returnSegment;
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
