#include "PathData.h"
#include "Segment.h"
#include "Location.h"

namespace Shipping {
  Segment::Ptr PathData::nextSegment(Location::Ptr location) {
    for (auto segment : segments()) {
      if (segment->source() == location) {
        return segment;
      }
    }

    return NULL;
  }
}
