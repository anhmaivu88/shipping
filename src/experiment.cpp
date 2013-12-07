#include <string>
#include <ostream>
#include <iostream>
#include <vector>
#include <math.h>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;

inline static std::string str(int n){ return std::to_string(n); }

static void badInstanceManager();

int main(int argc, char *argv[]) {
  Ptr<Instance::Manager> manager = shippingInstanceManager();

  if (manager == NULL) {
    badInstanceManager();
    return 1;
  }

  std::vector<Ptr<Instance>> allSegments;

  // Connect customer to root terminal
  Ptr<Instance> destination = manager->instanceNew("destination", "Customer");
  Ptr<Instance> root = manager->instanceNew("rootTerminal", "Truck terminal");

  Ptr<Instance> deliverySegment = manager->instanceNew("deliverySeg", "Truck segment");
  deliverySegment->attributeIs("source", root->name());

  Ptr<Instance> deliveryBak = manager->instanceNew("deliveryBak", "Truck segment");
  deliveryBak->attributeIs("source", destination->name());

  deliverySegment->attributeIs("return segment", deliveryBak->name());

  allSegments.push_back(deliverySegment);
  allSegments.push_back(deliveryBak);

  for(int i = 0; i < 10; i++){
    // Connect terminal to root
    Ptr<Instance> terminal = manager->instanceNew("terminal" + str(i), "Truck terminal");
    Ptr<Instance> segment = manager->instanceNew("seg" + str(i), "Truck segment");
    segment->attributeIs("length", "1");
    segment->attributeIs("source", root->name());

    Ptr<Instance> backseg = manager->instanceNew("backseg" + str(i), "Truck segment");
    backseg->attributeIs("length", "1");
    backseg->attributeIs("source", terminal->name());

    segment->attributeIs("return segment", backseg->name());
    allSegments.push_back(segment);
    allSegments.push_back(backseg);

    // Connect sources to terminal
    for(int j = 0; j < 10; j++){
      Ptr<Instance> source = manager->instanceNew("source" + str(i) + "," + str(j), "Customer");
      Ptr<Instance> sourceSegment = manager->instanceNew("seg" + str(i) + "," + str(j), "Truck segment");
      sourceSegment->attributeIs("length", "1");
      sourceSegment->attributeIs("source", source->name());

      Ptr<Instance> sourceBackseg = manager->instanceNew("backseg" + str(i) + "," + str(j), "Truck segment");
      sourceBackseg->attributeIs("length", "1");
      sourceBackseg->attributeIs("source", terminal->name());

      sourceSegment->attributeIs("return segment", sourceBackseg->name());

      allSegments.push_back(sourceSegment);
      allSegments.push_back(sourceBackseg);

      // Configure sources
      if(argc > 1 && std::string(argv[1]) == "--randomize"){
        source->attributeIs("Shipment Size", str(floor(rand() % 1000) + 1));
      } else {
        source->attributeIs("Shipment Size", "100");
      }
      source->attributeIs("Destination", destination->name());
      source->attributeIs("Transfer Rate", "1");
    }
  }

  Ptr<Instance> activityManager = manager->instanceNew("activity manager", "Activity Manager");

  cout << "Network complete. Beginning simulation..." << endl;

  activityManager->attributeIs("step", "0");
  activityManager->attributeIs("speed", "0");
  activityManager->attributeIs("time", "10000");
  
  cout << "Done!" << endl;
  
  cout << "Received shipments: " << deliverySegment->attribute("Shipments Received") << endl;

  double totalRefused = 0.0;
  for(Ptr<Instance> seg : allSegments){
    totalRefused += atof(seg->attribute("Shipments Refused").c_str());
  }
  cout << "Refused shipments (delivery segment) : " << deliverySegment->attribute("Shipments Refused") << endl;
  cout << "Refused shipments (all segments) : " << totalRefused << endl;
  cout << "Delivered shipments: " << destination->attribute("Shipments Received") << endl;
  cout << "Average latency: " << destination->attribute("Average Latency") << endl;
  cout << "Total cost: " << destination->attribute("Total Cost") << endl;

  return 0;
}

static void badInstanceManager() {
  cerr << "Unexpected Null pointer from shippingInstanceManager" << endl;
}
