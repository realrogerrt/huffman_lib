#include "huffman/core.h"

namespace huffman {

void key_map::feed_node(const __frequency_node& np) {
  if (this->symbols.count(np.__value) == 0) {
    this->symbols[np.__value].__value = np.__value;
    this->symbols[np.__value].__frequency = 1;
  } else {
    this->symbols[np.__value].__frequency++;
  }
}

__node_ptr key_map::build_tree() {
  priority_queue<__node_ptr, vector<__node_ptr>, less<__node_ptr>> q;
  for (auto it = symbols.begin(); it != symbols.end(); it++) {
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
  this->root = root;
  
  return root;

}






}  // namespace huffman