#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "Instance.h"
#include "Engine.h"
#include "Location.h"
#include "Statistics.h"
#include "Connectivity.h"
#include "Error.h"
#include "Query.h"


namespace Shipping {

  using namespace std;

  //
  // Rep layer classes
  //

  class FleetRep;
  class StatisticsRep;
  class ConnectivityRep;
  
  class ManagerImpl : public Instance::Manager {
  public:
    ManagerImpl(){
      engine_ = Engine::engineNew(string("engine"));
      statistics_ = Statistics::statisticsNew("primary statistics recorder", engine_);
      connectivity_ = Connectivity::connectivityNew("routing table generator", engine_);
    }

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

    Engine::Ptr engine() { return engine_; }
    Connectivity::Ptr connectivity() { return connectivity_; }
  
  private:
    map<string,Ptr<Instance> > instance_;
    Engine::Ptr engine_;
    Statistics::Ptr statistics_;
    Connectivity::Ptr connectivity_;

    Ptr<FleetRep> fleetRep_;
    Ptr<StatisticsRep> statisticsRep_;
    Ptr<ConnectivityRep> connectivityRep_;
  };

  class InstanceImpl : public Instance {
  public:
    enum class Status {
      DELETED,
        ACTIVE
        };

    Status status() { return status_; }
    void statusIs(Status status) { 
      if (status_ != Status::DELETED) { status_ = status; } 
      if (status_ == Status::DELETED) { deleteSelf(); } 
    }
    
  protected: 
    virtual void deleteSelf() = 0;
    InstanceImpl(const string &name) : Instance(name), status_(Status::ACTIVE) {}
    Status status_;

    static void printDeletedError() { 
      cerr << "Attempted to access attribute of deleted instance." << endl; 
      throw new MissingInstanceException("Cannot access attribute of deleted instance");
    }
  };

  class LocationRep : public InstanceImpl {
  public:

    LocationRep(const string& name, ManagerImpl* manager, Location::Ptr location) :
      InstanceImpl(name), manager_(manager), location_(location)
    {
      // Nothing else to do.
    }

    string attribute(const string& name) {
      if (status() == Status::DELETED) { printDeletedError(); return ""; }

      if (location()->type() == Location::Type::CUSTOMER) {
        Fwk::Ptr<Customer> customer = Fwk::ptr_cast<Customer, Location>(location());
        if (name == "Transfer Rate") {
          return customer->transferRate();
        } else if (name == "Shipment Size") {
          return customer->shipmentSize();
        } else if (name == "Destination") {
          if (customer->destination() != (Location::Ptr) NULL) {
            return customer->destination()->name();
          } else {
            return "";
          }
        }
      }

      if (name == "Shipments Received") {
        return location()->shipmentsReceived();
      } else if (name == "Average Latency") {
        return location()->averageLatency();
      } else if (name == "Total Cost") {
        return location()->totalCost();
      }

      int i = segmentNumber(name);
      if (i != 0) {
        Ptr<Segment> segment = location_->segment(i - 1);
        if (segment) {
          return segment->name();
        } else {
          cerr << "Segment [" << i << "] was not found for location [" << location()->name() << "]" << endl;
          throw new MissingInstanceException("Segment not found");
        }
      } else {
        cerr << "Invalid attribute [" << name << "] for location [" << location()->name() << "]" << endl;
        throw new InvalidAttributeException("Invalid attribute for Location");
      }
      return "";
    }


    void attributeIs(const string& name, const string& v) {
      if (location()->type() != Location::Type::CUSTOMER) {
        cerr << "Non-customer locations have no writable attributes." << endl;
        throw new InvalidAttributeException("Invalid attribute for Location");
      }

      Ptr<Customer> customer = Fwk::ptr_cast<Customer, Location>(location());

      if (name == "Transfer Rate") {
        customer->transferRateIs(atof(v.c_str()));
      } else if (name == "Shipment Size") {
        customer->shipmentSizeIs(atof(v.c_str()));
      } else if (name == "Destination") {
        if (v == "") { customer->destinationIs(NULL); }

        Ptr<Location> destinationLocation = manager_->engine()->location(v);
        if (destinationLocation != (Ptr<Location>) NULL) {
          customer->destinationIs(destinationLocation);
        } else {
          cerr << "Location not found." << endl;
          throw MissingInstanceException("Location not found.");
        }
      } else {
        cerr << "Unknown attribute " << name << " for location." << endl;
        throw InvalidAttributeException("Attribute not found for Location.");
      }
    }

  protected:
    Ptr<ManagerImpl> manager_;
    Location::Ptr location_;
    Location::Ptr location() { return location_; }

    int segmentNumber(const string& name);

    virtual void deleteSelf() { 
      manager_->engine()->locationDel(location_->name());
      location_ = NULL;
    }
  };
                                                                                                  
  class TerminalRep : public LocationRep {
  public:

    TerminalRep(const string& name, ManagerImpl *manager, Terminal::Ptr location) :
      LocationRep(name, manager, location), segmentType_(location->segmentType())
    {
      // Nothing else to do.
    }

  private:
    Segment::Type segmentType_;

  };

  class SegmentRep : public InstanceImpl {
  public:
    SegmentRep(const string& name, ManagerImpl* manager, Segment::Ptr segment) :
      InstanceImpl(name), manager_(manager), segment_(segment)
    {
      // Nothing else to do.
    }

    string attribute(const string& name){
      if (status() == Status::DELETED) { printDeletedError(); return ""; }

      if (name == "source") {
        if (segment()->source())
          return segment()->source()->name();
        else
          return "";
      } else if (name == "length") {
        return segment()->length();
      } else if (name == "return segment") {
        if (segment()->returnSegment())
          return segment()->returnSegment()->name();
        else
          return "";
      } else if (name == "difficulty") {
        return segment()->difficulty();
      } else if (name == "expedite support") {
        if (segment()->priority() == Segment::Priority::EXPEDITED) {
          return "yes";
        } else {
          return "no";
        }
      } else if (name == "Shipments Refused") {
        return segment()->shipmentsRefused();
      } else if (name == "Shipments Received") {
        return segment()->shipmentsReceived();
      } else if (name == "Capacity") {
        return segment()->capacity();
      } else {
        cerr << "Invalid attribute " << name << " for segment." << endl;
        throw InvalidAttributeException("Attribute not found for Segment.");
      }
    }

    void attributeIs(const string& name, const string& v){
      if (status() == Status::DELETED) { printDeletedError(); return; }

      if (name == "source") {
        if (v == "") { return segment()->sourceIs(NULL); }
        Ptr<Location> sourceLocation = manager_->engine()->location(v);
        if (sourceLocation == NULL){
          cerr << "Location not found." << endl;
          throw new MissingInstanceException("Location not found.");
        }
        return segment()->sourceIs(sourceLocation);
      } else if (name == "length") {
        return segment()->lengthIs(atof(v.c_str()));
      } else if (name == "return segment") {
        if(v == ""){
          segment()->returnSegmentIs(NULL);
        } else {
          Ptr<Segment> returnSegment = manager_->engine()->segment(v);
          if (returnSegment == NULL){
            cerr << "Segment not found." << endl;
            throw new MissingInstanceException("Segment not found.");
          }
          return segment()->returnSegmentIs(returnSegment);
        }
      } else if (name == "difficulty") {
        return segment()->difficultyIs(atof(v.c_str()));
      } else if (name == "expedite support") {
        if (v == "yes") {
          segment()->priorityIs(Segment::Priority::EXPEDITED);
        } else if (v == "no") {
          segment()->priorityIs(Segment::Priority::NORMAL);
        } else {
          cerr << "Expedited support must be `yes` or `no` for segment. Got " << v << endl;
          throw new InvalidAttributeException("Expedited attribute must be `yes` or `no`.");
        }
      } else if (name == "Capacity") {
        segment()->capacityIs(atof(v.c_str()));
      } else {
        cerr << "Unknown attribute " << name << " for segment." << endl;
        throw new InvalidAttributeException("Unknown attribute for Segment.");
      }
    }

  protected:
    Segment::Ptr segment() { return segment_; }
    Ptr<ManagerImpl> manager_;
    Segment::Ptr segment_;

    void deleteSelf() { 
      manager_->engine()->segmentDel(segment_->name());
      segment_ = NULL;
    }
  };

  class StatisticsRep : public InstanceImpl {
  public:
    StatisticsRep(const string& name, ManagerImpl* manager, Statistics::Ptr statistics) :
      InstanceImpl(name), manager_(manager), statistics_(statistics)
    {
    }

    string attribute(const string& name){
      if (status() == Status::DELETED) { printDeletedError(); return ""; }

      if (name == "Customer") {
        return to_string(statistics()->locationType(Location::customer()));
      } else if (name == "Port") {
        return to_string(statistics()->locationType(Location::port()));
      } else if (name == "Boat terminal") {
        return to_string(statistics()->locationType(Location::Type::BOAT_TERMINAL));
      } else if (name == "Truck terminal") {
        return to_string(statistics()->locationType(Location::Type::TRUCK_TERMINAL));
      } else if (name == "Plane terminal") {
        return to_string(statistics()->locationType(Location::Type::PLANE_TERMINAL));
      } else if (name == "Truck segment") {
        return to_string(statistics()->segmentType(Segment::truck()));
      } else if (name == "Boat segment") {
        return to_string(statistics()->segmentType(Segment::boat()));
      } else if (name == "Plane segment") {
        return to_string(statistics()->segmentType(Segment::plane()));
      } else if (name == "expedite percentage") {
        return statistics()->expeditedShippingPercentage();
      } else {
        cerr << "Invalid attribute name " << name << " for statistics." << endl;
        throw new InvalidAttributeException("Unknown attribute for Statistics.");
      }
      return "";
    }

    void attributeIs(const string& name, const string& v){
      cerr << "Statistics is read only." << endl;
      throw new InvalidAttributeException("Cannot write to Statistics.");
    }

  protected:
    Ptr<ManagerImpl> manager_;
    Statistics::Ptr statistics() { return statistics_; }
    Statistics::Ptr statistics_;

    void deleteSelf() { }
  };

  class ConnectivityRep : public InstanceImpl {
  public:
    ConnectivityRep(const string& name, ManagerImpl* manager, Connectivity::Ptr connectivity) :
      InstanceImpl(name), manager_(manager), connectivity_(connectivity)
    {
      // Nothing else to do.
    }

    string attribute(const string& name){
      if (status() == Status::DELETED) { printDeletedError(); return ""; }

      Engine::Ptr eng = manager_->engine();
      stringstream ss(name);
      string token;
      ss >> token;
      Query q;
      if(token == "connect"){
        q = Query(Query::Type::connect_);

        string rest = ss.str().substr(string("connect ").length());
        int delimiterIndex = rest.find(" : ");
        if(delimiterIndex == string::npos){
          cerr << "Connectivity query must include delimiter ' : '." << endl;
          throw new InvalidAttributeException("Query syntax not valid.");
        }
        Location::Ptr loc1 = eng->location(rest.substr(0, delimiterIndex));
        Location::Ptr loc2 = eng->location(rest.substr(delimiterIndex + 3));
        if(loc1 == NULL || loc2 == NULL){
          cerr << "Connectivity: requested location does not exist." << endl;
          throw new MissingInstanceException("Location not found.");
        }
        q.startIs(loc1);
        q.endIs(loc2);
      } else if(token == "explore"){
        q = Query(Query::Type::explore_);

        string rest = ss.str().substr(string("explore ").length());
        int delimiterIndex = rest.find(" : ");
        if(delimiterIndex == string::npos){
          cerr << "Connectivity query must include delimiter ' : '." << endl;
          throw new InvalidAttributeException("Query syntax not valid.");
        }
        Location::Ptr loc = eng->location(rest.substr(0, delimiterIndex));
        if(loc == NULL){
          cerr << "Connectivity: requested location does not exist." << endl;
          throw new MissingInstanceException("Location not found.");
        }

        q.startIs(loc);

        rest = rest.substr(delimiterIndex + 3);
        stringstream ssRest(rest);
        while(true){
          string attr;
          double val;
          ssRest >> attr;
          if(ssRest.fail()) break;
          if(attr == "expedited"){
            q.expeditedIs(Segment::Priority::EXPEDITED);
            continue;
          }

          ssRest >> val;
          if(ssRest.fail()){
            cerr << "Connectivity: explore attribute " << attr << " needs value." << endl;
            throw new InvalidAttributeException("Explore attribute has no value.");
          }
          
          if(attr == "distance"){
            q.distanceIs(Mile(val));
          } else if(attr == "cost"){
            q.costIs(Dollar(val));
          } else if(attr == "time"){
            q.timeIs(Hour(val));
          } else {
            cerr << "Connectivity: explore attribute " << attr << " is not valid." << endl;
            throw new InvalidAttributeException("Invalid explore attribute.");    
          }
        }
      } else {
        cerr << "Connectivity query must have be `connect` or `explore` type." << endl;
        throw new InvalidAttributeException("Invalid query type."); 
      }

      connectivity_->queryIs(q);
      return connectivity_->result();
    }

    void attributeIs(const string& name, const string& v){
      // Connectivity has no mutators
    }

  protected:
    Ptr<ManagerImpl> manager_;
    Connectivity::Ptr connectivity_;

    void deleteSelf() { }
  };

  class ActivityManagerRep : public InstanceImpl {
  public:
    ActivityManagerRep(const string &name, ManagerImpl *manager) : InstanceImpl(name), manager_(manager) {}

    string attribute(const string& name) {
      if (name == "time") {
        return manager_->engine()->activityManager()->now();
      } else if(name == "step"){
        return manager_->engine()->activityManager()->step();
      } else if(name == "speed"){
        return manager_->engine()->activityManager()->speed();
      }
      
      return "";
    }

    void attributeIs(const string &name, const string &v) {
      if (name == "time") {
        if(!simulationStarted){
          finalizeRoutes();
          simulationStarted = true;
        }
        manager_->engine()->activityManager()->nowIs(atof(v.c_str()));
      } else if(name == "step"){
        manager_->engine()->activityManager()->stepIs(atof(v.c_str()));
      } else if(name == "speed"){
        manager_->engine()->activityManager()->speedIs(atof(v.c_str()));
      }
    }

  protected:
    Ptr<ManagerImpl> manager_;
    bool simulationStarted;
    void deleteSelf() { } 

    // Assembles the routing table.
    void finalizeRoutes(){
      auto locations = manager_->engine()->locations();
      for(auto p1 : locations){
        Location::Ptr me = p1.second;
        for(auto p2 : locations){
          Location::Ptr dest = p2.second;
          if(me != dest){
            cout << "Building route from " << me->name() << " to " << dest->name() << endl;
            Query query(Query::connect_);
            query.startIs(me);
            query.endIs(dest);
            manager_->connectivity()->queryIs(query);
            vector<PathData> routes = manager_->connectivity()->paths();
            cout << routes.size() << " routes found." << endl;
            for(PathData route : routes){
              me->routeIs(dest->name(), route);
            }
          }
        }
      }
    }
  };

  class FleetRep : public InstanceImpl {
  public:
    FleetRep(const string& name, ManagerImpl* manager) :
      InstanceImpl(name), manager_(manager)
    {}

    string attribute(const string& name) {
      if (status() == Status::DELETED) { printDeletedError(); return ""; }

      int delimiterIndex = name.find(", ");
      string fleetName = name.substr(0, delimiterIndex);
      string attrName = name.substr(delimiterIndex + 2);
      Fleet::Ptr fleet = this->fleet(fleetName);

      if (fleet == NULL) {
        cerr << "Invalid fleet name " << fleetName << "." << endl;
        throw new InvalidAttributeException("Invalid fleet name");
      }

      if (attrName == "speed") {
        return fleet->speed();
      } else if (attrName == "cost") {
        return fleet->cost();
      } else if (attrName == "capacity") {
        return fleet->capacity();
      } else { 
        cerr << "Invalid attribute name " << attrName << endl;
        throw new InvalidAttributeException("Invalid attribute name");
      }
    }

    void attributeIs(const string& name, const string& v){
      if (status() == Status::DELETED) { printDeletedError(); return; }

      int delimiterIndex = name.find(", ");
      string fleetName = name.substr(0, delimiterIndex);
      string attrName = name.substr(delimiterIndex + 2);
      Fleet::Ptr fleet = this->fleet(fleetName);

      if (fleet == NULL) {
        cerr << "Invalid fleet name " << fleetName << "." << endl;
        throw new InvalidAttributeException("Invalid fleet name");
      }

      if (attrName == "speed") {
        fleet->speedIs(atof(v.c_str()));
      } else if (attrName == "cost") {
        fleet->costIs(atof(v.c_str()));
      } else if (attrName == "capacity") {
        fleet->capacityIs(atof(v.c_str()));
      } else { 
        cerr << "Invalid attribute name " << attrName << endl;
        throw new InvalidAttributeException("Invalid attribute name");
      }
    }

  protected:
    Fleet::Ptr fleet(EntityName name) { return manager_->engine()->fleet(name); }
    Ptr<ManagerImpl> manager_;

    void deleteSelf() { }
  };

  Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if(instance(name) != NULL){
      cerr << "Instance name already in use." << endl;
      throw new MissingInstanceException("Instance cannot be created; name in use");
    }

    Ptr<Instance> t;
    if(type == "Customer"){ // locations
      t = new LocationRep(name, this, engine_->customerLocationNew(name));
    } else if(type == "Port") {
      t = new LocationRep(name, this, engine_->portLocationNew(name));
    } else if(type == "Truck terminal") {
      t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::truck()));
    } else if(type == "Boat terminal"){
      t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::boat()));
    } else if(type == "Plane terminal"){
      t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::plane()));
    } else if(type == "Truck segment"){ // segments
      t = new SegmentRep(name, this, engine_->truckSegmentNew(name));
    } else if(type == "Boat segment"){
      t = new SegmentRep(name, this, engine_->boatSegmentNew(name));
    } else if(type == "Plane segment"){
      t = new SegmentRep(name, this, engine_->planeSegmentNew(name));
    } else if(type == "Activity Manager") {
      t = new ActivityManagerRep(name, this);
    } else if(type == "Fleet"){
      if(fleetRep_) { 
        fleetRep_->statusIs(InstanceImpl::Status::ACTIVE);
      } else {
        fleetRep_ = new FleetRep(name, this);
      }
      t = fleetRep_;
    } else if(type == "Stats"){
      if(statisticsRep_) { 
        statisticsRep_->statusIs(InstanceImpl::Status::ACTIVE);
      } else {
        statisticsRep_ = new StatisticsRep(name, this, statistics_);
      }
      t = statisticsRep_;
    } else if(type == "Conn"){
      if(connectivityRep_) { 
        connectivityRep_->statusIs(InstanceImpl::Status::ACTIVE); 
      } else {
        connectivityRep_ = new ConnectivityRep(name, this, Connectivity::connectivityNew(name, engine_));
      }
      t = connectivityRep_;
    } else {
      cerr << "Invalid instance type." << endl;
      throw new MissingInstanceException("Instance type does not exist");
    }

    instance_[name] = t;
    return t;
  }

  Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

    return t == instance_.end() ? NULL : (*t).second;
  }

  void ManagerImpl::instanceDel(const string& name) {
    Ptr<Instance> targetInstance = instance(name);
    if (targetInstance == NULL) {
      cerr << "Attempted to delete non-existent instance [" << name << "]." << endl;
      throw new InvalidDeletionTarget("Cannot delete non-existent instance");
    }

    /* This is safe. The only way that we can be deleting a named instance in this method
       is if it was initially constructed through instanceNew -- which will be creating an InstanceImpl. */
    Ptr<InstanceImpl> instanceImpl = Fwk::ptr_cast<InstanceImpl, Instance>(targetInstance);
    instanceImpl->statusIs(InstanceImpl::Status::DELETED);
    instance_.erase(name);
  }

  static const string segmentStr = "segment";
  static const int segmentStrlen = segmentStr.length();

  int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
      const char* t = name.c_str() + segmentStrlen;
      return atoi(t);
    }
    return 0;
  }
}

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
  return new Shipping::ManagerImpl();
}

