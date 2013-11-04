#include "Engine.h"

namespace Shipping {
  Segment::Ptr Engine::planeSegmentNew(EntityName name){
  	return segmentNew(name, Segment::plane());
  }

  Segment::Ptr Engine::boatSegmentNew(EntityName name){
  	return segmentNew(name, Segment::boat());
  }

  Segment::Ptr Engine::truckSegmentNew(EntityName name){
  	return segmentNew(name, Segment::truck());
  }

  Segment::Ptr Engine::segmentNew(EntityName name, Segment::Type type) {
    Segment::Ptr segment = Segment::segmentNew(name, type);
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
