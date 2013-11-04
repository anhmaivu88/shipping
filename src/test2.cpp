#include "Engine.h"
#include <map>
#include <iostream>

using namespace std;
using namespace Shipping;

int main(int argc, char *argv[]) {
  map<EntityName, Location::Ptr> testMap;
  Port::Ptr testPort = Port::portNew("Yo");
  testMap["Yo"] = testPort;
  cout << testMap["Yo"]->type();
}
