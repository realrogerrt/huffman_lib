#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn) : __file_name(fn) {}

void compressor::__build_key_tree() {
  ifstream is(__file_name, ios::binary | ios::in);
  __frequency_node n;

  while (is >> n) {
    __key_map_ref.__feed_node(n);
  }
  __node_ptr root = __key_map_ref.__build_tree();

  cout << *root << endl;
}

void compressor::compress() {
  __build_key_tree();

  ifstream is(__file_name, ios::binary | ios::in);

  uint8_t symbol;

  while (is >> symbol) {
    auto p = __key_map_ref.translate(symbol);

    // cout << static_cast<char>(symbol) << ":" << std::hex
    //      << static_cast<int>(symbol) << ":" << std::dec
    //      << static_cast<int>(p.first) << ":" << std::dec
    //      << static_cast<int>(p.second) << endl;
  }

  is.close();
}

}  // namespace huffman