#include "hello.hpp"

#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  Kokkos::initialize(argc, argv);
  {
    // use kokkos parallel_for to print hello world from each thread
    Kokkos::parallel_for(
        10, KOKKOS_LAMBDA(const int i) {
          auto h = hello::hello();
          std::cout << h.get_message() << " from thread " << i << std::endl;
        });
  }
  Kokkos::finalize();
  return EXIT_SUCCESS;
}