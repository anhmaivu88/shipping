#include "Location.h"

#ifndef TERMINAL_H
#define TERMINAL_H
namespace Shipping {
  class Terminal : public Shipping::Location {
  public:
    typedef Fwk::Ptr<Terminal> Ptr;

    static Ptr terminalNew(EntityName name, Segment::Type segmentType){
      Ptr ptr = new Terminal(name, segmentType);
      return ptr;
    }

    void segmentIs(int index, SegmentPtr segment){ 
      /* FIXME: handle this error. */
      if (segment->type() != segmentType()) return;
      segments_[index] = segment;
    }

    void segmentAdd(SegmentPtr segment) {
      /* FIXME: handle this error. */
      if (segment->type() != segmentType()) return;
      segments_.push_back(segment);
    }

    Segment::Type segmentType(){ return segmentType_; }
    void segmentTypeIs(Segment::Type segmentType){
      if(segments_.size() > 0) throw new ValueOutOfBoundsException("can't change type of terminal with attached segments");
      segmentType_ = segmentType;
    }

  protected:
    Segment::Type segmentType_;
    Segment::Type segmentType() { return segmentType_; }

    Terminal(EntityName name, Segment::Type segmentType) : Shipping::Location(name, terminal()), segmentType_(segmentType){}
	};
}
#endif
