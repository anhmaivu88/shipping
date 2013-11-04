#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>

#include "Instance.h"
#include "Engine.h"
#include "Location.h"
#include "Statistics.h"
#include "Connectivity.h"


namespace Shipping {

using namespace std;

//
// Rep layer classes
//
class ManagerImpl;

class InstanceImpl : public Instance {
public:
    InstanceImpl(const string& name, Engine::Ptr engine):
        Instance(name), engine_(engine)
    {
        // Done
    }
protected:
    Engine::Ptr engine_;
};

class StatisticsRep : public InstanceImpl {
public:
    StatisticsRep(const string& name, Engine::Ptr engine, Statistics::Ptr statistics) :
        InstanceImpl(name, engine), statistics_(statistics)
    {
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Statistics::Ptr statistics_;
};

class ConnectivityRep : public InstanceImpl {
public:
    ConnectivityRep(const string& name, Engine::Ptr engine, Connectivity::Ptr connectivity) :
        InstanceImpl(name, engine), connectivity_(connectivity)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Connectivity::Ptr connectivity_;
};

class FleetRep : public InstanceImpl {
public:
    FleetRep(const string& name, Engine::Ptr engine, Fleet::Ptr fleet) :
        InstanceImpl(name, engine), fleet_(fleet)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Fleet::Ptr fleet_;
};

class ManagerImpl : public Instance::Manager {
public:
    ManagerImpl(){
        engine_ = Engine::engineNew(string("engine"));
    }

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

private:
    map<string,Ptr<Instance> > instance_;
    Engine::Ptr engine_;

    Ptr<FleetRep> fleetRep_;
    Ptr<StatisticsRep> statisticsRep_;
    Ptr<ConnectivityRep> connectivityRep_;
};

class LocationRep : public InstanceImpl {
public:

    LocationRep(const string& name, Engine::Ptr engine, Location::Ptr location) :
        InstanceImpl(name, engine), location_(location)
    {
        // Nothing else to do.
    }

    string attribute(const string& name) {
        int i = segmentNumber(name);
        if (i != 0) {
            return location_->segment(i)->name();
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

    TerminalRep(const string& name, Engine::Ptr engine, Terminal::Ptr location) :
        LocationRep(name, engine, location), segmentType_(location->segmentType())
    {
        // Nothing else to do.
    }

private:
    Segment::Type segmentType_;

};

class SegmentRep : public InstanceImpl {
public:
    SegmentRep(const string& name, Engine::Ptr engine, Segment::Ptr segment) :
        InstanceImpl(name, engine), segment_(segment)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        stringstream ss;
        if(name == "source"){
            return segment_->source()->name();
        } else if(name == "length"){
            ss << setprecision(2);
            ss << segment_->length().value();
        } else if(name == "return segment"){
            Segment::Ptr ret = segment_->returnSegment();
            if(ret) return ret->name();
            return "";
        } else if(name == "difficulty"){
            ss << setprecision(1);
            ss << segment_->difficulty().value();
        } else if(name == "expedite support"){
            if(segment_->priority() == Segment::expedited()) return "yes";
            return "no";
        } else return "";

        return ss.str();
    }

    void attributeIs(const string& name, const string& v){
        stringstream ss(name);
        if(name == "source"){
            Location::Ptr loc = engine_->location(v);
            if(loc) segment_->sourceIs(loc);
        } else if(name == "length"){
            double len;
            ss >> len;
            try {
                segment_->lengthIs(Mile(len));
            } catch(...) {}
        } else if(name == "return segment"){
            Segment::Ptr ret = engine_->segment(v);
            if(ret) segment_->returnSegmentIs(ret);
        } else if(name == "difficulty"){
        } else if(name == "expedite support"){
        }
    }

protected:
    Segment::Ptr segment_;
};

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if(instance(name) != NULL){
        cerr << "Instance name already in use." << endl;
        return NULL;
    }

    Ptr<Instance> t;
    if(type == "Customer"){ // locations
        t = new LocationRep(name, engine_, engine_->customerLocationNew(name));
    } else if(type == "Port") {
        t = new LocationRep(name, engine_, engine_->portLocationNew(name));
    } else if(type == "Truck terminal") {
        t = new TerminalRep(name, engine_, engine_->terminalLocationNew(name, Segment::truck()));
    } else if(type == "Boat terminal"){
        t = new TerminalRep(name, engine_, engine_->terminalLocationNew(name, Segment::boat()));
    } else if(type == "Plane terminal"){
        t = new TerminalRep(name, engine_, engine_->terminalLocationNew(name, Segment::plane()));
    } else if(type == "Truck segment"){ // segments
        t = new SegmentRep(name, engine_, engine_->truckSegmentNew(name));
    } else if(type == "Boat segment"){
        t = new SegmentRep(name, engine_, engine_->boatSegmentNew(name));
    } else if(type == "Plane segment"){
        t = new SegmentRep(name, engine_, engine_->planeSegmentNew(name));
    } else if(type == "Fleet"){
        if(fleetRep_) return fleetRep_;
        fleetRep_ = new FleetRep(name, engine_, engine_->fleetNew(name));
        t = fleetRep_;
    } else if(type == "Stats"){
        if(statisticsRep_) return statisticsRep_;
        statisticsRep_ = new StatisticsRep(name, engine_, Statistics::statisticsNew(name, engine_));
        t = statisticsRep_;
    } else if(type == "Conn"){
        if(connectivityRep_) return connectivityRep_;
        connectivityRep_ = new ConnectivityRep(name, engine_, Connectivity::connectivityNew(name, engine_));
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
