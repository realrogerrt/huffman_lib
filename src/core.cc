#include "huffman/core.h"

namespace huffman {
    std::ostream& operator << (std::ostream& os, const __frequency_node& n)
    {
      return os << "value=" << n.__value << " " << "frequence=" << n.__frequency;
    }

    std::ifstream& operator >> (std::ifstream& is, uint8_t& n)
    {
      char c;
      is >> c;
      n = static_cast<uint8_t>(c);
      return is;
    }

    std::ifstream& operator >> (std::ifstream& is, __frequency_node& n)
    {
      char ch;
      is >> ch;
      // is >> n.__value;
      n.__value = static_cast<uint8_t>(ch);
      n.__frequency = 1;
      return is;
    }
}
