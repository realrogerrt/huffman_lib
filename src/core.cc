#include "huffman/core.h"

namespace huffman {

// __frequency_node::~__frequency_node() { delete this->parent; }
bool operator>(const __frequency_node& a, const __frequency_node& b) {
  return a.__frequency > b.__frequency;
}

void __frequency_node::__merge(__frequency_node& other) {
  other.parent = this;
  this->__frequency += other.__frequency;
}

std::ostream& operator<<(std::ostream& os, const __frequency_node& n) {
  return os << "value=" << n.__value << " "
            << "frequence=" << n.__frequency;
}

std::ifstream& operator>>(std::ifstream& is, __frequency_node& n) {
  char ch;
  is >> ch;
  // is >> n.__value;
  n.__value = static_cast<uint8_t>(ch);
  n.__frequency = 1;
  return is;
}
}  // namespace huffman
