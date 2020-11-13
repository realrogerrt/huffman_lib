#include "huffman/core.h"

namespace huffman {

std::ifstream& operator>>(std::ifstream& is, uint8_t& v) {
    char char_val;
    is >> char_val;

    v = static_cast<uint8_t>(char_val);

    return is;
}

}