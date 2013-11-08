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
    }

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

    Engine::Ptr engine() { return engine_; }
  
  private:
    map<string,Ptr<Instance> > instance_;
    Engine::Ptr engine_;
    Statistics::Ptr statistics_;

    Ptr<FleetRep> fleetRep_;
    Ptr<StatisticsRep> statisticsRep_;
    Ptr<ConnectivityRep> connectivityRep_;
  };



  class LocationRep : public Instance {
  public:

    LocationRep(const string& name, ManagerImpl* manager, Location::Ptr location) :
      Instance(name), manager_(manager), location_(location)
    {
      // Nothing else to do.
    }

    string attribute(const string& name) {
      int i = segmentNumber(name);
      if (i != 0) {
        return location_->segment(i - 1)->name();
      }
      return "";
    }


    void attributeIs(const string& name, const string& v) {
      // Mothing to do
    }

  protected:
    Ptr<ManagerImpl> manager_;
    Location::Ptr location_;

    int segmentNumber(const string& name);

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

  class SegmentRep : public Instance {
  public:
    SegmentRep(const string& name, ManagerImpl* manager, Segment::Ptr segment) :
      Instance(name), manager_(manager), segment_(segment)
    {
      // Nothing else to do.
    }

    string attribute(const string& name){
      if (name == "source") {
        return segment()->source()->name();
      } else if (name == "length") {
        return segment()->length();
      } else if (name == "return segment") {
        return segment()->returnSegment()->name();
      } else if (name == "difficulty") {
        return segment()->difficulty();
      } else if (name == "expedite support") {
        if (segment()->priority() == Segment::Priority::EXPEDITED) {
          return "yes";
        } else {
          return "no";
        }
      } else {
        cerr << "Invalid attribute " << name << " for segment." << endl;
        return "";
      }
    }

    void attributeIs(const string& name, const string& v){
      try {
        if (name == "source") {
          Ptr<Location> sourceLocation = manager_->engine()->location(v);
          if (sourceLocation == NULL) throw new MissingInstanceException("Location not found.");
          return segment()->sourceIs(sourceLocation);
        } else if (name == "length") {
          return segment()->lengthIs(atof(v.c_str()));
        } else if (name == "return segment") {
          Ptr<Segment> returnSegment = manager_->engine()->segment(v);
          if (returnSegment == NULL) throw new MissingInstanceException("Segment not found.");
          return segment()->returnSegmentIs(returnSegment);
        } else if (name == "difficulty") {
          return segment()->difficultyIs(atof(v.c_str()));
        } else if (name == "expedite support") {
          if (v == "yes") {
            segment()->priorityIs(Segment::Priority::EXPEDITED);
          } else if (v == "no") {
            segment()->priorityIs(Segment::Priority::NORMAL);
          } else {
            cerr << "Expedited support must be `yes` or `no` for segment. Got " << v << endl;
          }
        } else {
          cerr << "Unknown attribute " << name << " for segment." << endl;
        }
      } catch (...) {
        cerr << "Invalid value [" << v << "] for attribute: [" << name << "]" << endl;
      }
    }

  protected:
    Segment::Ptr segment() { return segment_; }
    Ptr<ManagerImpl> manager_;
    Segment::Ptr segment_;
  };

  class StatisticsRep : public Instance {
  public:
    StatisticsRep(const string& name, ManagerImpl* manager, Statistics::Ptr statistics) :
      Instance(name), manager_(manager), statistics_(statistics)
    {
    }

    string attribute(const string& name){
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
      }
      return "";
    }

    void attributeIs(const string& name, const string& v){
      cerr << "Statistics is write only." << endl;
    }

  protected:
    Ptr<ManagerImpl> manager_;
    Statistics::Ptr statistics() { return statistics_; }
    Statistics::Ptr statistics_;
  };

  class ConnectivityRep : public Instance {
  public:
    ConnectivityRep(const string& name, ManagerImpl* manager, Connectivity::Ptr connectivity) :
      Instance(name), manager_(manager), connectivity_(connectivity)
    {
      // Nothing else to do.
    }

    string attribute(const string& name){
      Engine::Ptr eng = manager_->engine();
      stringstream ss(name);
      string token;
      ss >> token;
      Query q;
      try {
        if(token == "connect"){
          q = Query(Query::Type::connect_);

          ss >> token;
          Location::Ptr loc1 = eng->location(token);

          ss >> token;
          if(token != ":") throw new InvalidAttributeException("Query syntax not valid.");

          ss >> token;
          Location::Ptr loc2 = eng->location(token);
          if(loc1 == NULL || loc2 == NULL) throw new MissingInstanceException("Location not found.");

          ss >> token;
          if(!ss.fail()) throw new InvalidAttributeException("Query syntax not valid.");

          q.startIs(loc1);
          q.endIs(loc2);
        } else if(token == "explore"){
          q = Query(Query::Type::explore_);

          ss >> token;
          Location::Ptr loc = eng->location(token);
          if(loc == NULL) throw new MissingInstanceException("Location not found.");

          ss >> token;
          if(token != ":") throw new InvalidAttributeException("Query syntax not valid.");

          q.startIs(loc);
          while(true){
            string attr;
            double val;
            ss >> attr;
            if(ss.fail()) break;
            if(attr == "expedited"){
              q.expeditedIs(Segment::Priority::EXPEDITED);
              continue;
            }

            ss >> val;
            if(ss.fail()) throw new InvalidAttributeException("Explore attribute has no value.");
            
            if(attr == "distance"){
              q.distanceIs(Mile(val));
            } else if(attr == "cost"){
              q.costIs(Dollar(val));
            } else if(attr == "time"){
              q.timeIs(Hour(val));
            } else throw new InvalidAttributeException("Invalid explore attribute.");    
          }
        } else throw new InvalidAttributeException("Invalid query type."); 
      } catch(...){
        cerr << "Invalid connectivity query " << name << endl;
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
  };

  class FleetRep : public Instance {
  public:
    FleetRep(const string& name, ManagerImpl* manager) :
      Instance(name), manager_(manager)
    {
      manager->engine()->fleetNew("Boat");
      manager->engine()->fleetNew("Truck");
      manager->engine()->fleetNew("Plane");
    }

    string attribute(const string& name) {
      int delimiterIndex = name.find(", ");
      string fleetName = name.substr(0, delimiterIndex);
      string attrName = name.substr(delimiterIndex + 2);
      Fleet::Ptr fleet = this->fleet(fleetName);

      if (fleet == NULL) {
        cerr << "Invalid fleet name " << fleetName << "." << endl;
        return "";
      }

      if (attrName == "speed") {
        return fleet->speed();
      } else if (attrName == "cost") {
        return fleet->cost();
      } else if (attrName == "capacity") {
        return fleet->capacity();
      } else { 
        cerr << "Invalid attribute name " << attrName << endl;
        return "";
      }
    }

    void attributeIs(const string& name, const string& v){
      int delimiterIndex = name.find(", ");
      string fleetName = name.substr(0, delimiterIndex);
      string attrName = name.substr(delimiterIndex + 2);
      Fleet::Ptr fleet = this->fleet(fleetName);

      if (fleet == NULL) {
        cerr << "Invalid fleet name " << fleetName << "." << endl;
        return;
      }

      if (attrName == "speed") {
        fleet->speedIs(atof(v.c_str()));
      } else if (attrName == "cost") {
        fleet->costIs(atof(v.c_str()));
      } else if (attrName == "capacity") {
        fleet->capacityIs(atof(v.c_str()));
      } else { 
        cerr << "Invalid attribute name " << attrName << endl;
      }
    }

  protected:
    Fleet::Ptr fleet(EntityName name) { return manager_->engine()->fleet(name); }
    
    Ptr<ManagerImpl> manager_;
  };

  Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if(instance(name) != NULL){
      cerr << "Instance name already in use." << endl;
      return NULL;
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
    } else if(type == "Fleet"){
      if(fleetRep_) return fleetRep_;
      fleetRep_ = new FleetRep(name, this);
      t = fleetRep_;
    } else if(type == "Stats"){
      if(statisticsRep_) return statisticsRep_;
      statisticsRep_ = new StatisticsRep(name, this, statistics_);
      t = statisticsRep_;
    } else if(type == "Conn"){
      if(connectivityRep_) return connectivityRep_;
      connectivityRep_ = new ConnectivityRep(name, this, Connectivity::connectivityNew(name, engine_));
      t = connectivityRep_;
    } else {
      cerr << "Invalid instance type." << endl;
      return NULL;
    }

    instance_[name] = t;
    return t;
  }

  Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

    return t == instance_.end() ? NULL : (*t).second;
  }

  void ManagerImpl::instanceDel(const string& name) {
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
