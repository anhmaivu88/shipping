#include "gtest/gtest.h"
#include "Engine.h"
#include "Segment.h"
#include "Location.h"
#include "Statistics.h"
#include "Error.h"

using namespace Shipping;

class EngineTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    engine = Engine::engineNew("Testing engine");
    statistics = Statistics::statisticsNew("Stats", engine);
  }

  Engine::Ptr engine;
  Statistics::Ptr statistics;
};


TEST_F(EngineTest, TestNameExists) {
  ASSERT_EQ(engine->name(), "Testing engine");
}

TEST_F(EngineTest, EngineShouldStartEmpty) {
  ASSERT_EQ(engine->location("Nonexistant location"), (Location::Ptr) NULL);
  ASSERT_EQ(engine->segment("Nonexistant segment"), (Segment::Ptr) NULL);
}

TEST_F(EngineTest, ShouldBeAbleToAddSegments) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Segment::Ptr boatSegment = engine->boatSegmentNew("Boat segment");
  Segment::Ptr planeSegment = engine->planeSegmentNew("Plane segment");
  ASSERT_EQ(engine->segment("Truck segment"), truckSegment);
  ASSERT_EQ(engine->segment("Boat segment"), boatSegment);
  ASSERT_EQ(engine->segment("Plane segment"), planeSegment);
  ASSERT_EQ(truckSegment->type(), Segment::Type::TRUCK);
  ASSERT_EQ(boatSegment->type(), Segment::Type::BOAT);
  ASSERT_EQ(planeSegment->type(), Segment::Type::PLANE);
}

TEST_F(EngineTest, ShouldBeAbleToAddLocations) {
  Location::Ptr truckTerminal = engine->terminalLocationNew("Truck terminal", Segment::Type::TRUCK);
  ASSERT_EQ(engine->location("Truck terminal"), truckTerminal);
  ASSERT_EQ(truckTerminal->type(), Location::Type::TRUCK_TERMINAL);
}

TEST_F(EngineTest, ShouldBeAbleToDeleteSegments) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  engine->segmentDel(truckSegment->name());
  ASSERT_EQ(engine->segment("Truck segment"), (Segment::Ptr) NULL);
}

TEST_F(EngineTest, ShouldBeAbleToDeleteLocations) {
  Location::Ptr truckTerminal = engine->terminalLocationNew("Truck terminal", Segment::Type::TRUCK);
  engine->locationDel(truckTerminal->name());
  ASSERT_EQ(engine->location("Truck terminal"), (Location::Ptr) NULL);
}

TEST_F(EngineTest, StatisticsShouldCountThings) {
  Location::Ptr truckTerminal = engine->terminalLocationNew("Truck terminal", Segment::Type::TRUCK);
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  
  ASSERT_EQ(statistics->locationType(Location::Type::TRUCK_TERMINAL), 1);
  ASSERT_EQ(statistics->segmentType(Segment::Type::TRUCK), 1);

  engine->locationDel(truckTerminal->name());
  engine->segmentDel(truckSegment->name());

  ASSERT_EQ(statistics->locationType(Location::Type::TRUCK_TERMINAL), 0);
  ASSERT_EQ(statistics->segmentType(Segment::Type::TRUCK), 0);
}

TEST_F(EngineTest, StatisticsShouldTrackExpeditedPercentage) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Segment::Ptr truckSegment2 = engine->truckSegmentNew("Truck segment2");
  
  ASSERT_EQ(statistics->expeditedShippingPercentage(), 0.0);
  truckSegment->priorityIs(Segment::Priority::EXPEDITED);
  ASSERT_EQ(statistics->expeditedShippingPercentage(), 0.5);
  truckSegment2->priorityIs(Segment::Priority::EXPEDITED);
  /* Make sure we don't double count. */
  truckSegment2->priorityIs(Segment::Priority::EXPEDITED);
  ASSERT_EQ(statistics->expeditedShippingPercentage(), 1.0);
  truckSegment2->priorityIs(Segment::Priority::NORMAL);
  ASSERT_EQ(statistics->expeditedShippingPercentage(), 0.5);
  engine->segmentDel(truckSegment2->name());
  ASSERT_EQ(statistics->expeditedShippingPercentage(), 1.0);
}

TEST_F(EngineTest, SettingReturnSegmentShouldEffectOtherSegments) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Segment::Ptr truckSegment2 = engine->truckSegmentNew("Truck segment2");
  Segment::Ptr truckSegment3 = engine->truckSegmentNew("Truck segment2");

  truckSegment->returnSegmentIs(truckSegment2);
  ASSERT_EQ(truckSegment2->returnSegment(), truckSegment);
  truckSegment3->returnSegmentIs(truckSegment);
  ASSERT_EQ(truckSegment->returnSegment(), truckSegment3);
  ASSERT_EQ(truckSegment2->returnSegment(), (Segment::Ptr) NULL);
}

TEST_F(EngineTest, LocationsShouldTrackSegments) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Location::Ptr port = engine->portLocationNew("port");
  truckSegment->sourceIs(port);

  ASSERT_EQ(truckSegment->source(), port);  
}

TEST_F(EngineTest, TerminalsShouldOnlyHaveOneKindOfSegment) {
  Terminal::Ptr boatTerminal = engine->terminalLocationNew("Boat terminal", Segment::Type::BOAT);
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Segment::Ptr boatSegment = engine->boatSegmentNew("Boat segment2");

  ASSERT_ANY_THROW(truckSegment->sourceIs(boatTerminal));
  ASSERT_EQ(truckSegment->source(), (Location::Ptr) NULL);
  ASSERT_NO_THROW(boatSegment->sourceIs(boatTerminal));
  ASSERT_EQ(boatSegment->source(), boatTerminal);
}

TEST_F(EngineTest, SegmentsShouldOnlyReturnAlongSameTypeSegments) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Segment::Ptr truckSegment2 = engine->truckSegmentNew("Truck segment2");
  Segment::Ptr boatSegment = engine->boatSegmentNew("Boat segment");

  ASSERT_NO_THROW(truckSegment->returnSegmentIs(truckSegment2));
  ASSERT_ANY_THROW(truckSegment->returnSegmentIs(boatSegment));
}

TEST_F(EngineTest, DeletingSourceShouldPropogateToSegments) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Location::Ptr port = engine->portLocationNew("port");
  truckSegment->sourceIs(port);

  engine->locationDel(port->name());
  ASSERT_EQ(truckSegment->source(), (Location::Ptr) NULL);
}

TEST_F(EngineTest, ShouldBeAllowedToReUseDeletedNamed) {
  Segment::Ptr truckSegment = engine->truckSegmentNew("Truck segment");
  Location::Ptr port = engine->portLocationNew("port");
  truckSegment->sourceIs(port);

  engine->locationDel(port->name());
  ASSERT_NO_THROW(port = engine->portLocationNew("port"));
  ASSERT_NO_THROW(truckSegment->sourceIs(port));
}
