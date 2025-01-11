#ifndef HELLO_HEADERDEF_TEST
#define HELLO_HEADERDEF_TEST

#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>

#include "hello.hpp"

// ----------------------------------------------------------------------------
// testing for the hello class
class helloTest : public ::testing::Test {
 protected:
  hello::hello _h;

  void SetUp() override {
    // code here will execute just before the test ensues
    _h = hello::hello();
  } 

};


#endif