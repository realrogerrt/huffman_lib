#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn) : __file_name(fn) {}

void compressor::__build_key_tree() {
  ifstream is(this->__file_name, ios::binary | ios::in);
  __frequency_node n;
  while (is >> n) {
    this->__key_map_ref.__feed_node(n);
  }

  __node_ptr root = this->__key_map_ref.__build_tree();

  cout << *root << endl;
}

}  // namespace huffman