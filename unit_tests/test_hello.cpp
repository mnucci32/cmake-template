#include "test_hello.hpp"

#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <sstream>

TEST_F(helloTest, test_hello_world) {
  // create a string stream to capture the output
  std::stringstream ss;
  // call the hello_world function
  _h.hello_world(ss);
  // check the output
  EXPECT_EQ(ss.str(), "Hello World!\n");
}

TEST_F(helloTest, test_get_vector) {
  // get the vector
  auto v = _h.get_vector();
  // check the vector
  EXPECT_EQ(v(0), 1);
  EXPECT_EQ(v(1), 2);
  EXPECT_EQ(v(2), 3);
}

TEST_F(helloTest, test_get_message) {
  // get the message
  auto message = _h.get_message();
  // check the message
  EXPECT_EQ(message, "Hello World!");
}