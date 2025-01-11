#ifndef HELLO_HEADERDEF
#define HELLO_HEADERDEF

#include <ostream>
#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>

namespace hello {

// class defining hello function
class hello {
  // public members
  Eigen::Vector3d _v;
  YAML::Node _node;

 public:
  // constructor
  hello() : _v(1,2,3) {
    _node = YAML::Load("message: Hello World!");
  }
  // destructor
  ~hello() = default;
  // function to print "Hello World!" to the console
  void hello_world(std::ostream &os) const {
    os << "Hello World!" << std::endl;
  }
  // function to get the vector
  const Eigen::Vector3d &get_vector() const {
    return _v;
  } 
  // function to get the message from the YAML node
  const std::string get_message() const {
    return _node["message"].as<std::string>();
  }
};

}  // end namespace hello

#endif