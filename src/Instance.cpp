#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "Instance.h"
#include "Engine.h"
#include "Location.h"
#include <sstream>

namespace Shipping {

using namespace std;

//
// Rep layer classes
//

class ManagerImpl : public Instance::Manager {
public:
    ManagerImpl();

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

private:
    map<string,Ptr<Instance> > instance_;
    Engine::Ptr engine_;
};

class LocationRep : public Instance {
public:

    LocationRep(const string& name, ManagerImpl* manager, Location::Ptr location) :
        Instance(name), manager_(manager), location_(location)
    {
        // Nothing else to do.
    }

    // Instance method
    string attribute(const string& name);

    // Instance method
    void attributeIs(const string& name, const string& v);

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


ManagerImpl::ManagerImpl() {
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if (type == "Truck terminal") {
        Ptr<TerminalRep> t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::truck()));
        instance_[name] = t;
        return t;
    }

    return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
}


string LocationRep::attribute(const string& name) {
    int i = segmentNumber(name);
    if (i != 0) {
        return location_->segment(i)->name();
    }
    return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
    //nothing to do
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
