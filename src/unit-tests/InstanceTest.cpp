#include "gtest/gtest.h"
#include "Instance.h"


class InstanceTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    manager = shippingInstanceManager();
  }
  
  Ptr<Instance::Manager> manager;
};


TEST_F(InstanceTest, TestingSimpleThings) {
  Ptr<Instance> instance = manager->instanceNew("test location", "Customer");

  ASSERT_NE(instance, (Ptr<Instance>) NULL);
  ASSERT_EQ(instance, manager->instance("test location"));
}
