#ifndef PATHDATA_H
#define PATHDATA_H

#include "Segment.h"

namespace Shipping {
	class PathData {
	public:
		PathData(std::vector<Segment::Ptr> segments, Mile distance, Dollar cost, Hour time, Segment::Priority priority) : 
			segments_(segments), distance_(distance), cost_(cost), time_(time), priority_(priority)
		{}

		std::vector<Segment::Ptr> segments(){ return segments_; }
	    Mile distance(){ return distance_; }
	    Dollar cost(){ return cost_; }
	    Hour time(){ return time_; }
	    Segment::Priority priority(){ return priority_; }

	private:
		std::vector<Segment::Ptr> segments_;
	    Mile distance_;
	    Dollar cost_;
	    Hour time_;
	    Segment::Priority priority_;
	};
}

#endif