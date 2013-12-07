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
  Ptr<Instance> secondterminal = manager->instanceNew("terminal2", "Customer");
  Ptr<Instance> anotherFirstTerminal = manager->instanceNew("terminal3", "Customer");

  if (a == NULL) {
    badTruckTerminal();
    return 1;
  }

  Ptr<Instance> b = manager->instanceNew("seg1", "Truck segment");
  Ptr<Instance> c = manager->instanceNew("seg2", "Truck segment");
  Ptr<Instance> d = manager->instanceNew("seg3", "Truck segment");
  Ptr<Instance> e = manager->instanceNew("seg4", "Truck segment");
  Ptr<Instance> f = manager->instanceNew("seg5", "Truck segment");
  Ptr<Instance> g = manager->instanceNew("seg6", "Truck segment");

  b->attributeIs("length", "1");
  c->attributeIs("length", "1");
  d->attributeIs("length", "1");
  e->attributeIs("length", "1");
  f->attributeIs("length", "1");
  g->attributeIs("length", "1");

  if (b == NULL || c == NULL) {
    badTruckSegment();
    return 1;
  }

  b->attributeIs("source", "customer1");
  c->attributeIs("source", "terminal1");
  d->attributeIs("source", "terminal1");
  e->attributeIs("source", "terminal2");
  f->attributeIs("source", "terminal3");
  g->attributeIs("source", "terminal1");
  d->attributeIs("return segment", "seg4");
  b->attributeIs("return segment", "seg2");
  f->attributeIs("return segment", "seg6");

  b->attributeIs("Capacity", "10");
  //f->attributeIs("Capacity", "5");
  d->attributeIs("Capacity", "1");

  customer->attributeIs("Transfer Rate", "1");
  customer->attributeIs("Shipment Size", "1000");
  customer->attributeIs("Destination", "terminal2");

  anotherFirstTerminal->attributeIs("Transfer Rate", "1");
  anotherFirstTerminal->attributeIs("Shipment Size", "1000");
  anotherFirstTerminal->attributeIs("Destination", "terminal2");

  Ptr<Instance> activityManagerRep = manager->instanceNew("activity manager", "Activity Manager");

  activityManagerRep->attributeIs("step", "0");
  activityManagerRep->attributeIs("speed", "0");
  activityManagerRep->attributeIs("time", "10001");
  
  cout << "Set all attributes." << endl;

  cout << "Done!" << endl;
  
  cout << "Received shipments: " << d->attribute("Shipments Received") << endl;
  cout << "Refused shipments: " << d->attribute("Shipments Refused") << endl;
  cout << "Delivered shipments: " << secondterminal->attribute("Shipments Received") << endl;
  cout << "Average latency: " << secondterminal->attribute("Average Latency") << endl;
  cout << "Total cost: " << secondterminal->attribute("Total Cost") << endl;

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
