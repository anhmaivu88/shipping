#ifndef PATHDATA_H
#define PATHDATA_H

#include <vector>
#include "Ptr.h"
#include "PtrInterface.h"
#include "Typedef.h"
#include "Mile.h"
#include "Dollar.h"
#include "Hour.h"


namespace Shipping {
	class PathData {
	public:

		PathData(std::vector<SegmentPtr> segments, Mile distance, Dollar cost, Hour time) ://, Segment::Priority priority) : 
			segments_(segments), distance_(distance), cost_(cost), time_(time)//, priority_(priority)
		{}

    SegmentPtr nextSegment(LocationPtr location);

		std::vector<SegmentPtr> segments(){ return segments_; }
	    Mile distance(){ return distance_; }
	    Dollar cost(){ return cost_; }
	    Hour time(){ return time_; }
	    // Segment::Priority priority(){ return priority_; }

	private:

		std::vector<SegmentPtr> segments_;
	    Mile distance_;
	    Dollar cost_;
	    Hour time_;
	    // Segment::Priority priority_;

	};
}

#endif
