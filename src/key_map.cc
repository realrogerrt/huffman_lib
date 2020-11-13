#include "huffman/core.h"

namespace huffman {

void __key_map::__reset() {
  __symbols.clear();
  if (__root) {
    delete __root;
  }
}

void __key_map::__feed_node(const __frequency_node& np) {
  if (__symbols.count(np.__value) == 0) {
    __symbols[np.__value].__value = np.__value;
    __symbols[np.__value].__frequency = 1;
  } else {
    __symbols[np.__value].__frequency++;
  }
}

__node_ptr __key_map::__build_tree() {
  priority_queue<__node_ptr, vector<__node_ptr>, less<__node_ptr>> q;

  for (auto it = __symbols.begin(); it != __symbols.end(); it++) {
    cout << it->second << endl;
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
    cout << q.size() << endl;

    merged = a->__merge(b);
    cout << "merged address=" << merged << endl;
    cout << "a address=" << a << endl;
    cout << "b address=" << b << endl;
    q.push(merged);
  }

  __node_ptr root = q.top();
  q.pop();
  __root = root;

  return root;
}

}  // namespace huffman