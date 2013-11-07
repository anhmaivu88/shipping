#include "gtest/gtest.h"
#include "Engine.h"

using namespace Shipping;

class EngineTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    engine = Engine::engineNew("Testing engine");
  }

  Engine::Ptr engine;
};


TEST_F(EngineTest, TestNameExists) {
  ASSERT_EQ(engine->name(), "Testing engine");
  ASSERT_EQ(engine->location("Nonexistant location"), (Location::Ptr) NULL);
}
