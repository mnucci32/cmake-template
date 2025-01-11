cmake_minimum_required (VERSION 3.26)
project (hello)
enable_testing() 

# set custom options
option(EIGEN_DIR "Path to Eigen installation to use instead of downloading" OFF)
option(YAML_CPP_DIR "Path to YAML-cpp installation to use instead of downloading" OFF)
option(KOKKOS_DIR "Path to kokkos installation" OFF)
option(BUILD_DOCS "Option to build user guide" OFF)
option(BUILD_DOCS_ONLY "Option to build only user guide" OFF)

# determine if we are building BUILD_DOCS
if (BUILD_DOCS OR BUILD_DOCS_ONLY)
  message(STATUS "Will attempt to build documentation")
  add_subdirectory(user_guide)
endif()

# we not just building docs build code
if (NOT BUILD_DOCS_ONLY)
  # add subdirectories
  add_subdirectory(src)
  add_subdirectory(unit_tests)

  # set C++ standard for all targets
  set_property(TARGET hello PROPERTY CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(STATUS "Using C++17")

  # set addtional compiler flags for each build type
  set(CMAKE_CXX FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -march=native -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb -Og -pg -DDEBUG")
  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} -O0 -pg -DNDEBUG")

  if (CMAKE_BUILD_TYPE MATCHES "release")
    message(STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_RELEASE})
  elseif (CMAKE_BUILD_TYPE MATCHES "debug")
    message(STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_DEBUG})
  elseif (CMAKE_BUILD_TYPE MATCHES ${CMAKE_CXX_FLAGS_PROFILE})
    message(STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_PROFILE})
  else()
    message(STATUS "C++ flags: " ${CMAKE_CXX_FLAGS})
  endif()

  # export compiler commands for clang-tidy
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  
endif()