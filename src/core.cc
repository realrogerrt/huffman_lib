#include "huffman/core.h"

namespace huffman {

bool operator<(const __frequency_node& a, const __frequency_node& b) {
  return a.__frequency < b.__frequency;
}


shared_ptr<__frequency_node> __frequency_node::__merge(
    __frequency_node& other) {

  __frequency_node n; // = {0, }
  n.__frequency = this->__frequency + other.__frequency;
  shared_ptr<__frequency_node> p(&n);

  p->__left = shared_ptr<__frequency_node>(this);
  p->__right = shared_ptr<__frequency_node>(&other);

  other.__parent = p;
  this->__parent = p;


  return p;
}

std::ostream& operator<<(std::ostream& os, const __frequency_node& n) {
  return os << "value=" << n.__value << " " << "frequency=" << n.__frequency << endl;
}

std::ifstream& operator>>(std::ifstream& is, __frequency_node& n) {

  char data;
  is >> data;

  n.__value = static_cast<uint8_t>(data);
  n.__frequency = 1;

  return is;
}


}