#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn, const string& ofn)
    : __file_name(fn), __out_file_name(ofn) {}

void compressor::__build_key_tree(ifstream& is) {
  frequency_node n;
  n.__frequency = 1;

  while (is.read((char*)&n.__value, sizeof(n.__value))) {
    __key_map_ref.__feed_node(n);
  }
  node_ptr root = __key_map_ref.__build_tree();
}

void compressor::expand() {
  ifstream is(__file_name, ios::binary | ios::in);
  ofstream os(__out_file_name, ios::binary | ios::out);

  is.seekg(0, is.end);
  size_t total_size = is.tellg();
  size_t total_chars = 0;
  is.seekg(0);
  size_t len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  frequency_node n;

  while (len > 0) {
    debug(is.tellg());
    is >> n;
    debug((int)n.__value);
    debug((char)n.__value);
    debug((int)n.__frequency);
    total_chars += n.__frequency;
    // cout << "====" << endl;
    __key_map_ref.__feed_node(n);
    len--;
    // cout << "last line in while";
  }

  // cout << "building tree" << endl;

  // debug(is.tellg());
  node_ptr root = __key_map_ref.__build_tree();
  // cout << "finished building tree" << endl;

  typedef frequency_node::symbol_type symbol_type;

  uint64_t partial_path;
  symbol_type symbol;
  size_t full_size = sizeof(partial_path) * 8;
  size_t bits_remaining_in_path = full_size;

  node_ptr node = nullptr;
  // debug(min(sizeof(partial_path), total_size - (size_t)is.tellg()))
  // debug(total_size);
  // debug(is.tellg());
  while (is.read(reinterpret_cast<char*>(&partial_path),
                 min(sizeof(partial_path), total_size - (size_t)is.tellg()))) {
    // debug(is.tellg());
    cout << "NEW PATH" << endl;
    while (bits_remaining_in_path > 0 && total_chars > 0) {
      debug(bits_remaining_in_path)
      print_binary(partial_path);
      // debug((int) partial_path)
      node = __key_map_ref.decode(partial_path, node, bits_remaining_in_path);
      // debug((int) partial_path)
      // debug(bits_remaining_in_path)
      if (node->leaf()) {
        // debug((int)node->__value);
        os.write((char*)&node->__value, sizeof(node->__value));
        total_chars--;
        // debug((char) node->__value)
        // debug(total_chars)
      }
    }

    debug(bits_remaining_in_path)
    bits_remaining_in_path = full_size;
    // debug(is.tellg());
    if (is.tellg() == total_size) {
      break;
    }

    // debug(min(sizeof(partial_path), total_size - (size_t)is.tellg()))
  }

  is.close();
  os.close();
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
  size_t remaining_bits = chunk_sz * 8;

  is.clear();
  is.seekg(0, is.beg);
  char* bin = new char[symbol_sz];
  while (is.read(bin, symbol_sz)) {
    symbol = (symbol_type)*bin;
    key_map::translation_pair p = __key_map_ref.encode(symbol);

    key_map::chunk_type symbol_chunk = p.first;
    // symbol_chunk <<= (symbol_bits - p.second);
    uint64_t extended_chunk = static_cast<uint64_t>(symbol_chunk);
    int overflow_bits = p.second - remaining_bits;
    size_t bits_for_next_chunk = max(overflow_bits, 0);
    // extended_chunk <<= (remaining_bits - symbol_bits + bits_for_next_chunk);
    extended_chunk <<= (chunk_bits - remaining_bits);
    cout << "=================" << endl;
    huffman::print_binary(chunk_to_write);
    huffman::print_binary(extended_chunk);
    chunk_to_write = chunk_to_write | extended_chunk;

    remaining_bits -= (p.second - bits_for_next_chunk);
    // debug(remaining_bits);

    if (remaining_bits == 0) {
      __write_and_reset(os, chunk_to_write, remaining_bits);
    }

    // TODO: All these 8's and 64's must come from generic info
    if (bits_for_next_chunk > 0) {
      debug(bits_for_next_chunk)
      symbol_chunk = p.first;
      // symbol_chunk = 4;
      cout << "prefilled chunk" << endl;
      print_binary(symbol_chunk);
      debug((symbol_bits - bits_for_next_chunk))
      // symbol_chunk >>= (symbol_bits - bits_for_next_chunk);
      symbol_chunk >>= (p.second - bits_for_next_chunk);
      print_binary(symbol_chunk);
      extended_chunk = static_cast<uint64_t>(symbol_chunk);
      // extended_chunk <<= (chunk_bits - symbol_bits);

      chunk_to_write = extended_chunk;
      debug(symbol)
      debug(symbol_chunk)
      debug(p.second)
      print_binary(symbol_chunk);
      print_binary(chunk_to_write);

      remaining_bits -= bits_for_next_chunk;
      debug(remaining_bits)
    }
  }
  delete[] bin;

  if (remaining_bits < chunk_bits) {
    __write_and_reset(os, chunk_to_write, remaining_bits);
  }

  is.close();
  os.close();
}
void compressor::__write_and_reset(ofstream& os, uint64_t& chunk,
                                   size_t& remaining) {
  size_t chunk_size = sizeof(chunk);
  size_t chunk_bits = chunk_size * 8;
  char* data_ptr = (char*)&chunk;
  size_t bytes_used = ceil((chunk_bits - remaining) / 8.0f);

  print_binary(chunk);
  debug(bytes_used);
  // debug(remaining);

  os.write(data_ptr, bytes_used);
  chunk = 0;
  remaining = chunk_bits;
}

}  // namespace huffman