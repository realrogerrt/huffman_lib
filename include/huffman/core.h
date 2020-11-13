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
#define debug(v) cout << #v << "=" << v << endl;

namespace huffman {

using namespace std;

void print_binary(uint64_t);

struct frequency_node {
  frequency_node* __parent;
  frequency_node* __left;
  frequency_node* __right;

  typedef uint8_t symbol_type;
  typedef size_t frequency_type;

  symbol_type __value;
  frequency_type __frequency;

  frequency_node* __merge(frequency_node*);

  bool leaf() { return !__left && !__right; }
};
typedef frequency_node* node_ptr;
typedef unordered_map<frequency_node::symbol_type, frequency_node> symbol_map;

bool operator>(const frequency_node&, const frequency_node&);

template <class _Pt> struct greater_ptr : binary_function<_Pt, _Pt, bool> {
  bool operator()(const _Pt& __x, const _Pt& __y) const { return *__x > *__y; }
};

std::ostream& operator<<(std::ostream&, const frequency_node&);

std::ifstream& operator>>(std::ifstream&, frequency_node&);

class key_map {
 private:
  node_ptr __root;
  symbol_map __symbols;

 public:
  typedef pair<size_t, size_t> translation_pair;
  typedef translation_pair::first_type chunk_type;
  typedef translation_pair::second_type count_type;

  void __feed_node(const frequency_node&);
  node_ptr __build_tree();
  void __write_head(ostream&);
  void __reset();
  translation_pair encode(frequency_node::symbol_type);
  node_ptr decode(uint64_t&, node_ptr, size_t&);
};

class compressor {
 private:
  string __file_name;
  string __out_file_name;
  key_map __key_map_ref;

 public:
  compressor(const string&, const string&);

  void __build_key_tree(ifstream&);
  void __write_and_reset(ofstream&, uint64_t&, size_t&);
  void compress();
  void expand();
};

}  // namespace huffman

#endif
