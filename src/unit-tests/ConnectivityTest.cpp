#include "gtest/gtest.h"
#include "Connectivity.h"
#include "Segment.h"
#include "Location.h"

using namespace Shipping;

class ConnectivityTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    engine = Engine::engineNew("Testing engine");
    engine->fleetNew(Fleet::boat());
    engine->fleetNew(Fleet::truck());
    engine->fleetNew(Fleet::plane());
    connectivity = Connectivity::connectivityNew("Conn", engine);
    nSegments = 0;
  }

  Segment::Ptr addSegmentsBetween(Location::Ptr src, Location::Ptr dest, Segment::Type type){
  	Segment::Ptr fwd;
  	Segment::Ptr bak;
  	if(type == Segment::Type::TRUCK){
  		fwd = engine->truckSegmentNew("Truck" + id());
  		bak = engine->truckSegmentNew("Truck" + id());
  	} else if(type == Segment::Type::BOAT){
  		fwd = engine->boatSegmentNew("Boat" + id());
  		bak = engine->boatSegmentNew("Boat" + id());
  	} else {
  		fwd = engine->planeSegmentNew("Plane" + id());
  		bak = engine->planeSegmentNew("Plane" + id());
  	}

  	fwd->sourceIs(src);
  	bak->sourceIs(dest);

  	fwd->returnSegmentIs(bak);
  	return fwd;
  }

  std::string id(){ return std::to_string(++nSegments); }

  Engine::Ptr engine;
  Connectivity::Ptr connectivity;
  size_t nSegments;
};

TEST_F(ConnectivityTest, ShouldHaveName){
	ASSERT_EQ(connectivity->name(), "Conn");
}

TEST_F(ConnectivityTest, ShouldStartEmpty){
	ASSERT_EQ(connectivity->result(), "");
}

TEST_F(ConnectivityTest, ShouldTraverseAllSegmentsBetweenTwoLocations){
	Location::Ptr port1 = engine->portLocationNew("port1");
  Location::Ptr port2 = engine->portLocationNew("port2");

  addSegmentsBetween(port1, port2, Segment::Type::BOAT);
  addSegmentsBetween(port1, port2, Segment::Type::PLANE);
  addSegmentsBetween(port1, port2, Segment::Type::TRUCK);

  Query q(Query::Type::connect_);
  q.startIs(port1);
  q.endIs(port2);
  connectivity->queryIs(q);
  std::stringstream ss(connectivity->result());
  int nLines = 1;
  while(ss) if(ss.get() == '\n') nLines++;
  ASSERT_EQ(nLines, 3);
}

TEST_F(ConnectivityTest, ShouldFindMultipleConnectPaths){
  Location::Ptr port1 = engine->portLocationNew("port1");
  Location::Ptr port2 = engine->portLocationNew("port2");
  Location::Ptr port3 = engine->portLocationNew("port3");

  addSegmentsBetween(port1, port2, Segment::Type::TRUCK);
  addSegmentsBetween(port1, port3, Segment::Type::TRUCK);
  addSegmentsBetween(port2, port3, Segment::Type::TRUCK);

  Query q(Query::Type::connect_);
  q.startIs(port1);
  q.endIs(port3);
  connectivity->queryIs(q);
  std::stringstream ss(connectivity->result());
  int nLines = 1;
  while(ss) if(ss.get() == '\n') nLines++;
  ASSERT_EQ(nLines, 2);
}

TEST_F(ConnectivityTest, ShouldFindMultipleExplorePaths){
  Location::Ptr port1 = engine->portLocationNew("port1");
  Location::Ptr port2 = engine->portLocationNew("port2");
  Location::Ptr port3 = engine->portLocationNew("port3");

  addSegmentsBetween(port1, port2, Segment::Type::TRUCK);
  addSegmentsBetween(port1, port3, Segment::Type::TRUCK);
  addSegmentsBetween(port2, port3, Segment::Type::TRUCK);

  Query q(Query::Type::explore_);
  q.startIs(port1);
  connectivity->queryIs(q);
  std::stringstream ss(connectivity->result());
  int nLines = 1;
  while(ss) if(ss.get() == '\n') nLines++;
  ASSERT_EQ(nLines, 4);
}

TEST_F(ConnectivityTest, ShouldNotTraverseThroughCustomers){
  Location::Ptr port1 = engine->portLocationNew("port1");
  Location::Ptr customer = engine->customerLocationNew("customer");
  Location::Ptr port2 = engine->portLocationNew("port2");

  addSegmentsBetween(port1, customer, Segment::Type::TRUCK);
  addSegmentsBetween(customer, port2, Segment::Type::TRUCK);

  Query q(Query::Type::connect_);
  q.startIs(port1);
  q.endIs(port2);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");
}

TEST_F(ConnectivityTest, ShouldNotConnectToSelf){
	Location::Ptr port1 = engine->portLocationNew("port1");
  Location::Ptr port2 = engine->portLocationNew("port2");

  addSegmentsBetween(port1, port2, Segment::Type::TRUCK);

  Query q(Query::Type::connect_);
  q.startIs(port1);
  q.endIs(port1);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");
}

TEST_F(ConnectivityTest, ShouldExploreEverything){
  Location::Ptr port1 = engine->portLocationNew("port1");
  Location::Ptr port2 = engine->portLocationNew("port2");
  Location::Ptr port3 = engine->portLocationNew("port3");

  addSegmentsBetween(port1, port2, Segment::Type::TRUCK);
  addSegmentsBetween(port1, port3, Segment::Type::TRUCK);

  addSegmentsBetween(port2, port3, Segment::Type::TRUCK);

  Query q(Query::Type::explore_);
  q.startIs(port1);
  connectivity->queryIs(q);
  std::stringstream ss(connectivity->result());
  int nLines = 1;
  // std::cout << connectivity->result() << std::endl;
  while(ss) if(ss.get() == '\n') nLines++;
  ASSERT_EQ(nLines, 4);
}

TEST_F(ConnectivityTest, ShouldLimitExplore){
  Location::Ptr port1 = engine->portLocationNew("port1");
	Location::Ptr port2 = engine->portLocationNew("port2");

  Segment::Ptr seg = addSegmentsBetween(port1, port2, Segment::Type::TRUCK);
  seg->lengthIs(Mile(9000));
  Query q(Query::Type::explore_);
  q.startIs(port1);

  q.distanceIs(500);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");

  q.distanceIs(10000);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result().length() > 0, true);

  q.costIs(10000);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result().length() > 0, true);

  engine->fleet(Fleet::truck())->costIs(2);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");

  q.costIs(20000);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result().length() > 0, true);

  q.timeIs(5000);
	connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");

  engine->fleet(Fleet::truck())->speedIs(2);
  connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result().length() > 0, true);

  seg->priorityIs(Segment::Priority::NORMAL);
  q.expeditedIs(Segment::Priority::EXPEDITED);
	connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");

  seg->priorityIs(Segment::Priority::EXPEDITED);
	connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result(), "");

	q.costIs(30000); // expedited is also more expensive
	connectivity->queryIs(q);
  ASSERT_EQ(connectivity->result().length() > 0, true);
}