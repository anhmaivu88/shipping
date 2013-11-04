#include "Engine.h"

namespace Shipping {
  Segment::Ptr Engine::planeSegmentNew(EntityName name){
  	Segment::Ptr segment = Segment::segmentNew(name, Segment::plane());
  	return segment;
  }

  Segment::Ptr Engine::boatSegmentNew(EntityName name){
  	Segment::Ptr segment = Segment::segmentNew(name, Segment::boat());
  	return segment;
  }

  Segment::Ptr Engine::truckSegmentNew(EntityName name){
  	Segment::Ptr segment = Segment::segmentNew(name, Segment::truck());
  	return segment;
  }

  Location::Ptr Engine::customerLocationNew(EntityName name){
  	Location::Ptr customer = Customer::customerNew(name);
  	return customer;
  }

  Location::Ptr Engine::portLocationNew(EntityName name){
  	Location::Ptr port = Port::portNew(name);
  	return port;
  }

  Terminal::Ptr Engine::terminalLocationNew(EntityName name, Segment::Type type) {
    Terminal::Ptr terminal = Terminal::terminalNew(name, type);
    return terminal;
  }

  Fleet::Ptr Engine::fleetNew(EntityName name) {
    Fleet::Ptr fleet = Fleet::fleetNew(name);
    return fleet;
  }
}
