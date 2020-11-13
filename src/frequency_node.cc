#include "huffman/core.h"

namespace huffman {

bool operator>(const __frequency_node& a, const __frequency_node& b) {
  return a.__frequency > b.__frequency;
}


__node_ptr __frequency_node::__merge(__node_ptr other) {
  __node_ptr n = new __frequency_node;
  n->__frequency = this->__frequency + other->__frequency;

  n->__left = this;
  n->__right = other;

  other->__parent = n;
  this->__parent = n;

  return n;
}

std::ostream& operator<<(std::ostream& os, const __frequency_node& n) {
  return os << "value=" << n.__value << " "
            << "frequency=" << n.__frequency;
}

std::ifstream& operator>>(std::ifstream& is, __frequency_node& n) {
  is >> n.__value;
  n.__frequency = 1;

  return is;
}

}  // namespace huffman