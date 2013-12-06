#include <string>
#include <ostream>
#include <iostream>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;

static void badInstanceManager();
static void badTruckTerminal();
static void badTruckSegment();

int main(int argc, char *argv[]) {
  Ptr<Instance::Manager> manager = shippingInstanceManager();

  if (manager == NULL) {
    badInstanceManager();
    return 1;
  }

  Ptr<Instance> customer = manager->instanceNew("customer1", "Customer");
  Ptr<Instance> a = manager->instanceNew("terminal1", "Truck terminal");
  Ptr<Instance> secondterminal = manager->instanceNew("terminal2", "Truck terminal");

  if (a == NULL) {
    badTruckTerminal();
    return 1;
  }

  Ptr<Instance> b = manager->instanceNew("seg1", "Truck segment");
  Ptr<Instance> c = manager->instanceNew("seg2", "Truck segment");
  Ptr<Instance> d = manager->instanceNew("seg3", "Truck segment");
  Ptr<Instance> e = manager->instanceNew("seg4", "Truck segment");

  b->attributeIs("length", "1");
  c->attributeIs("length", "1");
  d->attributeIs("length", "1");
  e->attributeIs("length", "1");

  if (b == NULL || c == NULL) {
    badTruckSegment();
    return 1;
  }

  b->attributeIs("source", "customer1");
  c->attributeIs("source", "terminal1");
  d->attributeIs("source", "terminal1");
  e->attributeIs("source", "terminal2");
  d->attributeIs("return segment", "seg4");
  b->attributeIs("return segment", "seg2");

  customer->attributeIs("Transfer Rate", "1");
  customer->attributeIs("Shipment Size", "1000");
  customer->attributeIs("Destination", "terminal2");

  Ptr<Instance> activityManagerRep = manager->instanceNew("activity manager", "Activity Manager");
  activityManagerRep->attributeIs("time", "10001");
  
  
  cout << "Set all attributes." << endl;

  cout << "Done!" << endl;

  return 0;
}

static void badInstanceManager() {
  cerr << "Unexpected Null pointer from shippingInstanceManager" << endl;
}

static void badTruckTerminal() {
  cerr << "Unable to create terminal1" << endl;
}

static void badTruckSegment() {
  cerr << "You need to update the "
    "instanceNew() function to create all types " << endl <<
    "of nodes and interfaces." << endl;
}
