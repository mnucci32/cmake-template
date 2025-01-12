#ifndef HELLO_HEADERDEF
#define HELLO_HEADERDEF

#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <ostream>

namespace hello {

// class defining hello function
class hello {
  // public members
  Eigen::Vector3d _v;
  YAML::Node _node;

 public:
  // constructor
  hello() : _v(1, 2, 3) { _node = YAML::Load("message: Hello World!"); }
  // copy constructor
  hello(const hello &h) = default;
  hello &operator=(const hello &h) = default;
  // move constructor
  hello(hello &&h) = default;
  hello &operator=(hello &&h) = default;
  // destructor
  ~hello() = default;
  // function to print "Hello World!" to the console
  void hello_world(std::ostream &os) const {
    os << "Hello World!" << std::endl;
  }
  // function to get the vector
  [[nodiscard]] const Eigen::Vector3d &get_vector() const { return _v; }
  // function to get the message from the YAML node
  [[nodiscard]] std::string get_message() const {
    return _node["message"].as<std::string>();
  }
};

}  // end namespace hello

#endif