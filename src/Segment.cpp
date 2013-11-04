#include "Segment.h"

namespace Shipping {

	void Segment::sourceIs(const LocationPtr source){
		if(source_ != source){
			if(source_){
				source_->segmentIs()
			}
			source_ = source;
			source->segmentIs(this);
		}
	}

	void Segment::returnSegmentIs(const Segment::Ptr segment){
		if(returnSegment_ != segment){
			returnSegment_ = segment;
			segment->returnSegmentIs(this);
		}
	}
}