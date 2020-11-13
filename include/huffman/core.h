#ifndef HUFFMAN_CORE_H_
#define HUFFMAN_CORE_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

// major * 10000 + minor * 100 + patch.
#define HUFFMAN_VERSION 00001

namespace huffman {

using namespace std;

struct __frequency_node {
  __frequency_node* __parent;
  __frequency_node* __left;
  __frequency_node* __right;

  uint8_t __value;
  uint32_t __frequency;

  __frequency_node* __merge(__frequency_node*);
};
typedef __frequency_node* __node_ptr;
typedef unordered_map<uint8_t, __frequency_node> __symbol_map;

bool operator>(const __frequency_node&, const __frequency_node&);

template <class _Pt> struct __greater_ptr : binary_function<_Pt, _Pt, bool> {
  bool operator()(const _Pt& __x, const _Pt& __y) const {
    return *__x > *__y;
  }
};

std::ostream& operator<<(std::ostream&, const __frequency_node&);

std::ifstream& operator>>(std::ifstream&, __frequency_node&);

std::ifstream& operator>>(std::ifstream&, uint8_t&);

class __key_map {
 private:
  __node_ptr __root;
  __symbol_map __symbols;

 public:
  typedef pair<uint8_t, uint8_t> __translation_pair;
  typedef __translation_pair::first_type chunk_type;
  typedef __translation_pair::second_type count_type;

  void __feed_node(const __frequency_node&);
  __node_ptr __build_tree();
  void __reset();
  __translation_pair translate(uint8_t symbol);
};

class compressor {
 private:
  string __file_name;
  string __out_file_name;
  __key_map __key_map_ref;

 public:
  compressor(const string&, const string&);

  void __build_key_tree();
  void __write_and_reset(ofstream&, uint64_t&, uint8_t&);
  void compress();
};

}  // namespace huffman

#endif
