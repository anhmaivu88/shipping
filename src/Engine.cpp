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
    SegmentReactor::Ptr segmentReactor = SegmentReactor::segmentReactorNew(this, segment.ptr());
    segmentIs(name, segment);
    return segment;
  }

  Location::Ptr Engine::customerLocationNew(EntityName name){
  	Location::Ptr customer = Customer::customerNew(name);
    locationIs(name, customer);
  	return customer;
  }

  Location::Ptr Engine::portLocationNew(EntityName name){
  	Location::Ptr port = Port::portNew(name);
    locationIs(name, port);
  	return port;
  }

  Terminal::Ptr Engine::terminalLocationNew(EntityName name, Segment::Type type) {
    Terminal::Ptr terminal;
    switch(type) {
    case Segment::Type::BOAT:
      terminal = BoatTerminal::boatTerminalNew(name);
      break;
    case Segment::Type::PLANE:
      terminal = PlaneTerminal::planeTerminalNew(name);
      break;
    case Segment::Type::TRUCK:
      terminal = TruckTerminal::truckTerminalNew(name);
      break;
    }
    locationIs(name, terminal);
    return terminal;
  }

  Fleet::Ptr Engine::fleetNew(EntityName name) {
    Fleet::Ptr fleet = Fleet::fleetNew(name);
    fleetIs(name, fleet);
    return fleet;
  }

  Fleet::Ptr Engine::fleet(EntityName name){
    auto it = fleets_.find(name);
    return it == fleets_.end() ? NULL : (*it).second;
  }

  Fleet::Ptr Engine::fleet(Segment::Type type){
    EntityName name;
    if(type == Segment::Type::BOAT) name = Fleet::boat();
    if(type == Segment::Type::TRUCK) name = Fleet::truck();
    if(type == Segment::Type::PLANE) name = Fleet::plane();
    else return NULL;
    return fleet(name);
  }

  void Engine::fleetIs(EntityName name, Fleet::Ptr fleet){
    fleets_[name] = fleet;
    for (auto notifiee : notifiees_) {
      try {
        notifiee->onFleetNew(name);
      } catch (...) { }
    }
  }

  Segment::Ptr Engine::segment(EntityName name){
    auto it = segments_.find(name);
    return it == segments_.end() ? NULL : (*it).second;
  }

  void Engine::segmentIs(EntityName name, Segment::Ptr segment) {
    segments_[name] = segment;
    for (auto notifiee : notifiees_) {
      try {
        notifiee->onSegmentNew(name);
      } catch (...) { }
    }
  }

  Location::Ptr Engine::location(EntityName name){
    auto it = locations_.find(name);
    return it == locations_.end() ? NULL : (*it).second;
  }

  void Engine::locationIs(EntityName name, Location::Ptr location) {
    locations_[name] = location;
    for (auto notifiee : notifiees_) {
      try {
        notifiee->onLocationNew(name);
      } catch (...) { }
    }
  }
}
