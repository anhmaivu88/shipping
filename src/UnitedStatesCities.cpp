#include <string>
#include <ostream>
#include <iostream>
#include "Instance.h"
#include <string>
#include <ostream>
#include <iostream>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
  Ptr<Instance::Manager> manager = shippingInstanceManager();
  Ptr<Instance> statistics = manager->instanceNew("Statistics", "Stats");

  Ptr<Instance> sanFrancisco = manager->instanceNew("San Francisco", "Port");
  Ptr<Instance> paloAlto = manager->instanceNew("Palo Alto", "Customer");
  Ptr<Instance> redwoodCity = manager->instanceNew("Redwood City", "Truck terminal");
  Ptr<Instance> seattle = manager->instanceNew("Seattle", "Customer");

  Ptr<Instance> southboundCaltrain1 = manager->instanceNew("Southbound Caltrain1", "Truck segment");
  Ptr<Instance> southboundCaltrain2 = manager->instanceNew("Southbound Caltrain2", "Truck segment");

  Ptr<Instance> northboundCaltrain1 = manager->instanceNew("Northbound Caltrain1", "Truck segment");
  Ptr<Instance> northboundCaltrain2 = manager->instanceNew("Northbound Caltrain2", "Truck segment");

  southboundCaltrain1->attributeIs("source", "San Francisco");
  southboundCaltrain2->attributeIs("source", "Redwood City");

  southboundCaltrain2->attributeIs("return segment", "Northbound Caltrain2");
  southboundCaltrain1->attributeIs("return segment", "Northbound Caltrain1");

  northboundCaltrain1->attributeIs("source", "Redwood City");
  northboundCaltrain2->attributeIs("source", "Palo Alto");

  Ptr<Instance> northPlane = manager->instanceNew("North plane", "Plane segment");
  Ptr<Instance> southPlane = manager->instanceNew("South plane", "Plane segment");

  northPlane->attributeIs("source", "San Francisco");
  southPlane->attributeIs("source", "Seattle");

  northPlane->attributeIs("return segment", "South plane");

  southboundCaltrain1->attributeIs("length", "20");
  southboundCaltrain2->attributeIs("length", "20");
  northboundCaltrain1->attributeIs("length", "20");
  northboundCaltrain2->attributeIs("length", "20");

  northboundCaltrain1->attributeIs("expedite support", "yes");
  northboundCaltrain2->attributeIs("expedite support", "yes");

  northPlane->attributeIs("length", "1000");
  southPlane->attributeIs("length", "1000");

  cout << "Total plane segments: " << statistics->attribute("Plane segment") << endl;
  cout << "Total truck segments: " << statistics->attribute("Truck segment") << endl;
  cout << "Percentage expedited shipping: " << statistics->attribute("expedite percentage") << endl;

  Ptr<Instance> conn = manager->instanceNew("Connection", "Conn");
  std::string query = "connect Seattle : Redwood City";
  cout << query << endl << conn->attribute(query) << endl;

  query = "explore Palo Alto : ";
  cout << query << endl << conn->attribute(query) << endl;

  query = "explore Palo Alto : distance 100";
  cout << query << endl << conn->attribute(query) << endl;
}
