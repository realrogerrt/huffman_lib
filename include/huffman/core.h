#ifndef HUFFMAN_CORE_H_
#define HUFFMAN_CORE_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <queue>
#include <iterator>
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
typedef shared_ptr<__frequency_node> node_ptr;
typedef __frequency_node* __node_ptr;
typedef unordered_map<uint8_t, __frequency_node> symbol_map;

bool operator<(const __frequency_node&, const __frequency_node&);

std::ostream& operator<<(std::ostream&, const __frequency_node&);

std::ifstream& operator>>(std::ifstream&, __frequency_node&);

class key_map {
  private:
    __node_ptr root;
    symbol_map symbols;

  public:
    void feed_node(const __frequency_node&);
    __node_ptr build_tree();
    void reset();


};

class compressor {

  private:
  string file_name;
  key_map key_map_ref;

  public:
  compressor(const string&);

  void __build_key_tree_old();
  void __build_key_tree();

};

}  // namespace huffman

#endif
