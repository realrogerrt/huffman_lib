#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn, const string& ofn)
    : __file_name(fn), __out_file_name(ofn) {}

void compressor::__build_key_tree(ifstream& is) {
  // ifstream is(__file_name, ios::binary | ios::in);
  __frequency_node n;

  while (is >> n) {
    __key_map_ref.__feed_node(n);
  }
  __node_ptr root = __key_map_ref.__build_tree();

  // cout << *root << endl;
}

void compressor::compress() {
  ifstream is(__file_name, ios::binary | ios::in);
  ofstream os(__out_file_name, ios::binary | ios::out);

  __build_key_tree(is);
  __key_map_ref.__write_head(os);

  uint8_t symbol;

  uint64_t chunk_to_write = 0;
  // TODO: This 64 constant must come from compile time limits
  uint8_t remaining_bits = 64;

  is.clear();
  is.seekg(0);
  while (is >> symbol) {
    __key_map::__translation_pair p = __key_map_ref.translate(symbol);
    // cout << static_cast<char>(symbol) << ":" << std::hex
    //      << static_cast<int>(symbol) << ":" << std::dec
    //      << static_cast<int>(p.first) << ":" << std::dec
    //      << static_cast<int>(p.second) << endl;

    __key_map::chunk_type symbol_chunk = p.first;
    // Moving 01 sequence to the front of the memory chunk
    // TODO: This 8 constant must come from compile time limits
    symbol_chunk <<= (8 - p.second);
    // Creating 64 bit sequence to make or
    uint64_t extended_chunk = static_cast<uint64_t>(symbol_chunk);
    // Calculating bits whom overflow
    uint8_t bits_for_next_chunk = max(p.second - remaining_bits, 0);
    // Positioning the 64 bits chunk in the correct place
    extended_chunk <<= (remaining_bits - 8 + bits_for_next_chunk);
    // Addedin new chunk to the chunk to be written eventually
    chunk_to_write = chunk_to_write | extended_chunk;

    remaining_bits -= (p.second - bits_for_next_chunk);

    if (remaining_bits == 0) {
      __write_and_reset(os, chunk_to_write, remaining_bits);
    }

    // TODO: All these 8's and 64's must come from generic info
    if (bits_for_next_chunk > 0)
    {
      symbol_chunk = p.first;
      symbol_chunk <<= (8 - bits_for_next_chunk);
      extended_chunk = static_cast<uint64_t>(symbol_chunk);
      extended_chunk <<= (64 - 8);

      chunk_to_write = extended_chunk;
      remaining_bits -= bits_for_next_chunk;
    }

  }

  if (remaining_bits < 64) {
    __write_and_reset(os, chunk_to_write, remaining_bits);
  }

  is.close();
  os.close();
}
void compressor::__write_and_reset(ofstream& os, uint64_t& chunk, uint8_t& remaining) {
  // cout << "chunk=" << std::hex << chunk << endl;
  char* data_ptr = (char*)&chunk;
  // TODO: Take all these constants in compile time from <numbers>
  uint8_t bytes_used = (64 - remaining) / 8;
  os.write(data_ptr, bytes_used);
  chunk = 0;
  remaining = 64;
}

}  // namespace huffman