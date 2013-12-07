#include "Location.h"
#include "Error.h"

#ifndef TERMINAL_H
#define TERMINAL_H
namespace Shipping {
  class Terminal : public Shipping::Location {
  public:
    typedef Fwk::Ptr<Terminal> Ptr;

    
    void segmentIs(int index, SegmentPtr segment){ 
      if (segment->type() != segmentType()){
        std::cerr << "Segment not compatible with terminal." << std::endl;
        throw new ValueOutOfBoundsException("Attempted to connect segment to incompatible terminal.");
      }
      segments_[index] = segment;
    }

    void segmentAdd(SegmentPtr segment) {
      if (segment->type() != segmentType()){
        std::cerr << "Segment not compatible with terminal." << std::endl;
        throw new ValueOutOfBoundsException("Attempted to connect segment to incompatible terminal.");
      }
      segments_.push_back(segment);
    }

    Segment::Type segmentType(){ return segmentType_; }
  protected:
    static Ptr terminalNew(EntityName name, Location::Type locationType, Segment::Type segmentType){
      Ptr ptr = new Terminal(name, locationType, segmentType);
      return ptr;
    }

    Terminal(EntityName name, Location::Type locationType, Segment::Type segmentType) : Shipping::Location(name, locationType), segmentType_(segmentType) {}

    Segment::Type segmentType_;
	};

  class BoatTerminal : public Shipping::Terminal {
  public:
    static Ptr boatTerminalNew(EntityName name) { Ptr ptr = Terminal::terminalNew(name, Location::Type::BOAT_TERMINAL, Segment::Type::BOAT); return ptr; }
  };

  class TruckTerminal : public Shipping::Terminal {
  public:
    static Ptr truckTerminalNew(EntityName name) { Ptr ptr = Terminal::terminalNew(name, Location::Type::TRUCK_TERMINAL, Segment::Type::TRUCK); return ptr; }
  };

  class PlaneTerminal : public Shipping::Terminal {
  public:
    static Ptr planeTerminalNew(EntityName name) { Ptr ptr = Terminal::terminalNew(name, Location::Type::PLANE_TERMINAL, Segment::Type::PLANE); return ptr; }
  };
}
#endif
