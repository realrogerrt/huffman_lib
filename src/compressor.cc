#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn, const string& ofn)
    : __file_name(fn), __out_file_name(ofn) {}

void compressor::__build_key_tree(ifstream& is) {
  frequency_node n;

  while (is >> n) {
    __key_map_ref.__feed_node(n);
  }
  node_ptr root = __key_map_ref.__build_tree();
}

void compressor::compress() {
  ifstream is(__file_name, ios::binary | ios::in);
  ofstream os(__out_file_name, ios::binary | ios::out);

  __build_key_tree(is);
  __key_map_ref.__write_head(os);

  typedef frequency_node::symbol_type symbol_type;

  symbol_type symbol;
  size_t symbol_sz = sizeof(symbol);
  size_t symbol_bits = symbol_sz * 8;

  uint64_t chunk_to_write = 0;
  size_t chunk_sz = sizeof(chunk_to_write);
  size_t chunk_bits = chunk_sz * 8;
  size_t remaining_bits = chunk_sz;

  is.clear();
  is.seekg(0);
  char* bin = new char[symbol_sz];
  while (is.read(bin, sizeof(bin))) {

    symbol = (symbol_type) *bin;
    key_map::translation_pair p = __key_map_ref.translate(symbol);
    // debug(p.second);
    // debug(remaining_bits);

    key_map::chunk_type symbol_chunk = p.first;
    symbol_chunk <<= (symbol_bits - p.second);
    uint64_t extended_chunk = static_cast<uint64_t>(symbol_chunk);
    int overflow_bits = p.second - remaining_bits;
    // debug(p.second - remaining_bits);
    // debug(overflow_bits);
    size_t bits_for_next_chunk = max(overflow_bits, 0);
    extended_chunk <<= (remaining_bits - symbol_bits + bits_for_next_chunk);
    chunk_to_write = chunk_to_write | extended_chunk;

    remaining_bits -= (p.second - bits_for_next_chunk);
    // debug(remaining_bits);

    if (remaining_bits == 0) {
      __write_and_reset(os, chunk_to_write, remaining_bits);
    }

    // TODO: All these 8's and 64's must come from generic info
    if (bits_for_next_chunk > 0)
    {
      symbol_chunk = p.first;
      symbol_chunk <<= (symbol_bits - bits_for_next_chunk);
      extended_chunk = static_cast<uint64_t>(symbol_chunk);
      extended_chunk <<= (chunk_bits - symbol_bits);

      chunk_to_write = extended_chunk;
      remaining_bits -= bits_for_next_chunk;
    }

  }
  delete [] bin;

  if (remaining_bits < chunk_bits) {
    __write_and_reset(os, chunk_to_write, remaining_bits);
  }

  is.close();
  os.close();
}
void compressor::__write_and_reset(ofstream& os, uint64_t& chunk, size_t& remaining) {
  size_t chunk_size = sizeof(chunk);
  size_t chunk_bits = chunk_size * 8;
  char* data_ptr = (char*)&chunk;
  size_t bytes_used = (chunk_bits - remaining) / 8;
  // debug(bytes_used);
  os.write(data_ptr, bytes_used);
  chunk = 0;
  remaining = chunk_bits;
}

}  // namespace huffman