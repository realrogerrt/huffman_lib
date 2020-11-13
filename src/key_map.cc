#include "huffman/core.h"

namespace huffman {

void __key_map::__reset() {
  __symbols.clear();
  if (__root) {
    delete __root;
  }
}

__key_map::__translation_pair __key_map::translate(__frequency_node::symbol_type symbol) {

  if (__symbols.count(symbol) == 0)
  {
    throw invalid_argument("Symbol not found");
  }


  __frequency_node symbol_node = __symbols[symbol];
  __node_ptr ptr = &symbol_node;
  __key_map::chunk_type chunk = 0;
  __key_map::count_type node_depth = 0;

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

void __key_map::__feed_node(const __frequency_node& np) {
  if (__symbols.count(np.__value) == 0) {
    __symbols[np.__value].__value = np.__value;
    __symbols[np.__value].__frequency = 1;
  } else {
    __symbols[np.__value].__frequency ++;
  }
}

void __key_map::__write_head(ostream& out) {
  uint8_t len = static_cast<uint8_t>(__symbols.size());
  char* len_ptr = (char*) &len;
  out.write(len_ptr, sizeof(len_ptr));
  for (auto it = __symbols.begin(); it != __symbols.end(); it++) {
    __frequency_node node = it->second;
    char* symbol = (char*)&node.__value;
    char* freq = (char*)&node.__frequency;

    out.write(symbol, sizeof(symbol));
    out.write(freq, sizeof(freq));
  }
}

__node_ptr __key_map::__build_tree() {

  priority_queue<__node_ptr, vector<__node_ptr>, __greater_ptr<__node_ptr>> q;

  for (auto it = __symbols.begin(); it != __symbols.end(); it++) {
    q.push(&it->second);
  }

  __node_ptr a = nullptr;
  __node_ptr b = nullptr;
  __node_ptr merged = nullptr;

  while (q.size() > 1) {
    a = q.top();
    q.pop();
    b = q.top();
    q.pop();

    merged = a->__merge(b);
    q.push(merged);
  }

  __node_ptr root = q.top();
  q.pop();
  __root = root;

  return root;
}

}  // namespace huffman