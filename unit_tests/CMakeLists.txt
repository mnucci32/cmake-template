# get google test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

# don't install along with the main project
set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# get cmake functions for google test
include(GoogleTest)

# get verbose test output on failure
list(APPEND ${CMAKE_CTEST_ARGUMENTS} "--output-on-failure")

# set all source files
set(hello_test_sources
  test_hello.cpp
)

# create targets for executable
add_executable(hello_test ${hello_test_sources})

# add include directories
target_include_directories(hello_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(hello_test PRIVATE ${CMAKE_SOURCE_DIR}/unit_tests/include)
target_include_directories(hello_test PRIVATE ${PROJECT_BINARY_DIR})
if(EIGEN_DIR)
  target_include_directories(hello_test PRIVATE ${EIGEN_DIR})
else()
  target_include_directories(hello_test PRIVATE ${PROJECT_BINARY_DIR}/_deps/eigen-src)
endif()


target_link_libraries(hello_test gtest gtest_main hello_static)
gtest_discover_tests(hello_test PROPERTIES LABELS "hello")