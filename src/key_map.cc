#include "huffman/core.h"

namespace huffman {

void key_map::__reset() {
  __symbols.clear();
  if (__root) {
    delete __root;
  }
}

key_map::translation_pair key_map::translate(frequency_node::symbol_type symbol) {

  if (__symbols.count(symbol) == 0)
  {
    throw invalid_argument("Symbol not found");
  }


  frequency_node symbol_node = __symbols[symbol];
  node_ptr ptr = &symbol_node;
  key_map::chunk_type chunk = 0;
  key_map::count_type node_depth = 0;

  while (ptr != __root) {
    chunk <<= 1;
    if (ptr->__parent->__left == ptr) {
      // chunk += 0;
    } else {
      chunk += 1;
    }
    node_depth++;
    ptr = ptr->__parent;
  }

  return make_pair(chunk, node_depth);
}

void key_map::__feed_node(const frequency_node& np) {
  if (__symbols.count(np.__value) == 0) {
    __symbols[np.__value].__value = np.__value;
    __symbols[np.__value].__frequency = 1;
  } else {
    __symbols[np.__value].__frequency ++;
  }
}

void key_map::__write_head(ostream& out) {
  uint8_t len = static_cast<uint8_t>(__symbols.size());
  char* len_ptr = (char*) &len;
  out.write(len_ptr, sizeof(len_ptr));
  for (auto it = __symbols.begin(); it != __symbols.end(); it++) {
    frequency_node node = it->second;
    char* symbol = (char*)&node.__value;
    char* freq = (char*)&node.__frequency;

    out.write(symbol, sizeof(symbol));
    out.write(freq, sizeof(freq));
  }
}

node_ptr key_map::__build_tree() {

  priority_queue<node_ptr, vector<node_ptr>, greater_ptr<node_ptr>> q;

  for (auto it = __symbols.begin(); it != __symbols.end(); it++) {
    q.push(&it->second);
  }

  node_ptr a = nullptr;
  node_ptr b = nullptr;
  node_ptr merged = nullptr;

  while (q.size() > 1) {
    a = q.top();
    q.pop();
    b = q.top();
    q.pop();

    merged = a->__merge(b);
    q.push(merged);
  }

  node_ptr root = q.top();
  q.pop();
  __root = root;

  return root;
}

}  // namespace huffman