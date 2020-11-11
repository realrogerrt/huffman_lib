#ifndef HUFFMAN_CORE_H_
#define HUFFMAN_CORE_H_

#include <cstdint>
#include <iostream>
#include <fstream>

// major * 10000 + minor * 100 + patch.
#define HUFFMAN_VERSION 00001

namespace huffman {

    struct __frequency_node
    {
        uint8_t __value;
        uint32_t __frequency;
    };

    std::ostream& operator << (std::ostream&, const __frequency_node&);
    // std::istream& operator >> (const std::istream&, __frequency_node&);
    std::ifstream& operator>> (std::ifstream& , __frequency_node& );
}

#endif
