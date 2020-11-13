#ifndef HUFFMAN_CORE_H_
#define HUFFMAN_CORE_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>


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

bool operator<(const __frequency_node&, const __frequency_node&);

std::ostream& operator<<(std::ostream&, const __frequency_node&);

std::ifstream& operator>>(std::ifstream&, __frequency_node&);

class __compressor {

  private:
  string file_name;

  public:
  __compressor(const string&);
  void __run(string);


};

}  // namespace huffman

#endif
