#include <string>
#include <ostream>
#include <iostream>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
  Ptr<Instance::Manager> manager = shippingInstanceManager();

  Ptr<Instance> origin = manager->instanceNew("customer1", "Customer");
  Ptr<Instance> firstTerminal = manager->instanceNew("terminal1", "Truck terminal");
  Ptr<Instance> origin2 = manager->instanceNew("customer2", "Customer");
  Ptr<Instance> destination = manager->instanceNew("destination_terminal", "Customer");

  Ptr<Instance> fleet = manager->instanceNew("fleet", "Fleet");

  fleet->attributeIs("Truck, speed", "1");
  
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

  b->attributeIs("source", "customer1");
  f->attributeIs("source", "customer2");

  c->attributeIs("source", "terminal1");
  d->attributeIs("source", "terminal1");
  e->attributeIs("source", "destination_terminal");
  g->attributeIs("source", "terminal1");

  b->attributeIs("return segment", "seg2");
  d->attributeIs("return segment", "seg4");
  f->attributeIs("return segment", "seg6");

  // segment b corresponds to C-B in the readme, and f is D-B. d is B-A
  b->attributeIs("Capacity", "1");
  f->attributeIs("Capacity", "1");
  d->attributeIs("Capacity", "10");

  origin->attributeIs("Transfer Rate", "24");
  origin->attributeIs("Shipment Size", "1000");
  origin->attributeIs("Destination", "destination_terminal");

  origin2->attributeIs("Transfer Rate", "24");
  origin2->attributeIs("Shipment Size", "1000");
  origin2->attributeIs("Destination", "destination_terminal");

  Ptr<Instance> activityManagerRep = manager->instanceNew("activity manager", "Activity Manager");

  activityManagerRep->attributeIs("step", "0");
  activityManagerRep->attributeIs("speed", "0");
  activityManagerRep->attributeIs("time", "10000");
  
  cout << "Set all attributes." << endl;

  cout << "Done!" << endl;
  
  cout << "Received shipments: " << d->attribute("Shipments Received") << endl;
  cout << "Refused shipments: " << d->attribute("Shipments Refused") << endl;
  cout << "Delivered shipments: " << destination->attribute("Shipments Received") << endl;
  cout << "Average latency: " << destination->attribute("Average Latency") << endl;
  cout << "Total cost: " << destination->attribute("Total Cost") << endl;

  return 0;
}
