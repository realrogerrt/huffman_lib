#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn) : file_name(fn) {}

void compressor::__build_key_tree() {
  ifstream is(this->file_name, ios::binary | ios::in);
  __frequency_node n;
  while (is >> n) {
    this->key_map_ref.feed_node(n);
  }

  __node_ptr root = this->key_map_ref.build_tree();

  cout << *root << endl;
}

}  // namespace huffman