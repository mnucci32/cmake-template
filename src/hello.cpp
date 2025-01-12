#include "hello.hpp"

#include <iostream>

int main() {
  auto h = hello::hello();
  h.hello_world(std::cout);
  return EXIT_SUCCESS;
}