# set version number from git
# run git describe to extract version information
execute_process(
  COMMAND git describe --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  RESULT_VARIABLE HAVE_GIT
  OUTPUT_VARIABLE GIT_DESCRIBE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# if we have git get version number from git
if (HAVE_GIT EQUAL 0)
  # parse version numbers from git describe output
  string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${GIT_DESCRIBE}")
  set(hello_VERSION_MAJOR ${CMAKE_MATCH_1})
  set(hello_VERSION_MINOR ${CMAKE_MATCH_2})
  set(hello_VERSION_PATCH ${CMAKE_MATCH_3})
  # configure header file to pass cmake variables
  configure_file (
    "${PROJECT_SOURCE_DIR}/include/hello_version.hpp.in"
    "${PROJECT_BINARY_DIR}/hello_version.hpp"
  )
endif()
# if we don't have git, hello_version.hpp should be included in release

message(STATUS "Cmake version ${CMAKE_VERSION}")
message(STATUS "Configuring hello version ${hello_VERSION_MAJOR}.${hello_VERSION_MINOR}.${hello_VERSION_PATCH}")
message(STATUS "Installation directory: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Include path: ${CMAKE_INCLUDE_PATH}")
message(STATUS "Library path: ${CMAKE_LIBRARY_PATH}")
message(STATUS "Prefix path: ${CMAKE_PREFIX_PATH}")
message(STATUS "Module path: ${CMAKE_MODULE_PATH}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# add include direcotries
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

# add source files
set(SOURCES
  hello.cpp
)

# create targest for executable and libraries
add_executable(hello ${SOURCES})
add_library(hello_shared SHARED ${SOURCES})
add_library(hello_static STATIC ${SOURCES})

# rename libraries
set_target_properties(hello_shared PROPERTIES OUTPUT_NAME hello)
set_target_properties(hello_static PROPERTIES OUTPUT_NAME hello)

# treate warnings as errors
option(HELLO_COMPILE_WARNINGS_AS_ERRORS "Compile with warnings as errors" OFF)
message(STATUS "HELLO_COMPILE_WARNINGS_AS_ERRORS: ${HELLO_COMPILE_WARNINGS_AS_ERRORS}")

if (HELLO_COMPILE_WARNINGS_AS_ERRORS)
  set_target_properties(hello PROPERTIES COMPILE_WARNING_AS_ERROR ON)
  set_target_properties(hello_shared PROPERTIES COMPILE_WARNING_AS_ERROR ON)
  set_target_properties(hello_static PROPERTIES COMPILE_WARNING_AS_ERROR ON)
endif()

# now get dependencies
include(FetchContent)
set(DEPENDENCY_TARGETS "")

# ---------------------------------------- Eigen --------------------------------------
# check if user provided Eigen or we need to download it
if(EIGEN_DIR)  # user provided Eigen
  # add the provide path to CMAKE_PREFIX_PATH so that find_package can locate it
  # prepend if it is already defined
  if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH ${EIGEN_DIR} CACHE STRING "Path to custom modules or packages")
  else()
    list(PREPEND CMAKE_PREFIX_PATH ${EIGEN_DIR})
  endif()
  message(STATUS "Looking for Eigen here: " ${CMAKE_PREFIX_PATH})
  find_package(Eigen3 3.4.0 REQUIRED NO_MODULE)
  set(eigen_SOURCE_DIR ${EIGEN_DIR})
else()  # download Eigen
  message(STATUS "Downloading Eigen")
  FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
  )
  FetchContent_MakeAvailable(eigen)
endif()

# ---------------------------------------- YAML-cpp --------------------------------------
# check if user provided YAML-cpp or we need to download it
if(YAML_CPP_DIR)  # user provided YAML-cpp
  # add the provide path to CMAKE_PREFIX_PATH so that find_package can locate it
  # prepend if it is already defined
  if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH ${YAML_CPP_DIR} CACHE STRING "Path to custom modules or packages")
  else()
    list(PREPEND CMAKE_PREFIX_PATH ${YAML_CPP_DIR})
  endif()
  message(STATUS "Looking for YAML-cpp here: " ${CMAKE_PREFIX_PATH})
  find_package(yaml-cpp 0.8.0 REQUIRED)
else()  # download YAML-cpp
  message(STATUS "Downloading YAML-cpp")
  FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
  )
  FetchContent_MakeAvailable(yaml-cpp)

  # install yaml-cpp if downloading it
  list(APPEND DEPENDENCY_TARGETS "yaml-cpp")
  install(TARGETS yaml-cpp
    EXPORT yaml-cpp-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
  )
  install(DIRECTORY ${yaml-cpp_SOURCE_DIR}/include/yaml-cpp
    DESTINATION include
  )
endif()

# ---------------------------------------- KOKKOS --------------------------------------
# add the provide path to CMAKE_PREFIX_PATH so that find_package can locate it
# prepend if it is already defined
if(NOT DEFINED CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH ${KOKKOS_DIR} CACHE STRING "Path to custom modules or packages")
else()
  list(PREPEND CMAKE_PREFIX_PATH ${KOKKOS_DIR})
endif()
message(STATUS "Looking for Kokkos here: " ${CMAKE_PREFIX_PATH})
find_package(Kokkos 4.5.01 REQUIRED)

# link to kokkos
target_link_libraries(hello Kokkos::kokkos)
target_link_libraries(hello_shared Kokkos::kokkos)
target_link_libraries(hello_static Kokkos::kokkos)

# link to yaml-cpp
target_link_libraries(hello yaml-cpp)
target_link_libraries(hello_shared yaml-cpp)
target_link_libraries(hello_static yaml-cpp)

# add eigen to include path
target_include_directories(hello PRIVATE ${eigen_SOURCE_DIR})
target_include_directories(hello_shared PRIVATE ${eigen_SOURCE_DIR})
target_include_directories(hello_static PRIVATE ${eigen_SOURCE_DIR})

# install executable, libraries, includes
install(TARGETS hello hello_shared hello_static ${DEPENDENCY_TARGETS}
  EXPORT hello_targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib 
  RUNTIME DESTINATION bin
)
FILE(GLOB files "${CMAKE_SOURCE_DIR}/include/*.hpp")
install(FILES ${files} DESTINATION include)
install(FILES "${PROJECT_BINARY_DIR}/hello_version.hpp" DESTINATION include)

# export targets for use by other projects
install(EXPORT hello_targets
  FILE hello_targets.cmake
  NAMESPACE hello::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hello
)

# generate config file for find_package
message(STATUS "Configuring package config files")
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/hello_config_version.cmake
  VERSION ${hello_VERSION_MAJOR}.${hello_VERSION_MINOR}.${hello_VERSION_PATCH}
  COMPATIBILITY SameMajorVersion
)

# configure the config.cmake file
configure_package_config_file(
  "${CMAKE_SOURCE_DIR}/cmake/hello_config.cmake.in"
  "${PROJECT_BINARY_DIR}/hello_config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hello
)

# install configuration and version files
install(FILES 
  "${PROJECT_BINARY_DIR}/hello_config.cmake"
  "${PROJECT_BINARY_DIR}/hello_config_version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hello
)