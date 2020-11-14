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
typedef __frequency_node* __node_ptr;
typedef unordered_map<uint8_t, __frequency_node> __symbol_map;

bool operator<(const __frequency_node&, const __frequency_node&);

std::ostream& operator<<(std::ostream&, const __frequency_node&);

std::ifstream& operator>>(std::ifstream&, __frequency_node&);

class __key_map {
  private:
    __node_ptr __root;
    __symbol_map __symbols;

  public:
    void __feed_node(const __frequency_node&);
    __node_ptr __build_tree();
    void __reset();


};

class compressor {

  private:
  string __file_name;
  __key_map __key_map_ref;

  public:
  compressor(const string&);

  void __build_key_tree();

};

}  // namespace huffman

#endif
