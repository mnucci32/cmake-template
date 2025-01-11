# include "hello.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    auto h = hello::hello();
    h.hello_world(std::cout);
    return EXIT_SUCCESS;
}