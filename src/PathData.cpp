#include "PathData.h"
#include "Segment.h"
#include "Location.h"

namespace Shipping {
  Segment::Ptr PathData::nextSegment(Location::Ptr location) {
    cout << "Number of segments: " << segments().size() << endl;
    for (auto segment : segments()) {
      if (segment->source() == location) {
        return segment;
      }
    }

    return NULL;
  }
}
