#include "gtest/gtest.h"
#include "Engine.h"
#include "Segment.h"
#include "Location.h"
#include "Statistics.h"

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
  
}
