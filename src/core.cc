#include "huffman/core.h"

namespace huffman {
void print_binary(uint64_t v) {
  size_t sz = sizeof(v) * 8;
  for (size_t i = 0; i < sz; i++) {
    cout << (v % 2);
    v >>= 1;
  }

  cout << endl;
}

}  // namespace huffman