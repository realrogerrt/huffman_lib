#include "huffman/core.h"

namespace huffman {

bool operator>(const frequency_node& a, const frequency_node& b) {
  return a.__frequency > b.__frequency;
}


node_ptr frequency_node::__merge(node_ptr other) {
  node_ptr n = new frequency_node;
  n->__frequency = this->__frequency + other->__frequency;

  n->__left = this;
  n->__right = other;

  other->__parent = n;
  this->__parent = n;

  return n;
}

std::ostream& operator<<(std::ostream& os, const frequency_node& n) {
  return os << "value=" << n.__value << " "
            << "frequency=" << n.__frequency;
}

std::ifstream& operator>>(std::ifstream& is, frequency_node& n) {
  char* bin = new char[sizeof(frequency_node::symbol_type)];
  is.read(bin, sizeof(bin));
  n.__value = (frequency_node::symbol_type) *bin;
  delete[] bin;

  bin = new char[sizeof(frequency_node::frequency_type)];
  is.read(bin, sizeof(bin));
  n.__frequency = (frequency_node::frequency_type) *bin;
  delete[] bin;

  return is;
}

}  // namespace huffman