
#include "huffman/core.h"

namespace huffman {

compressor::compressor(const string& fn) : file_name(fn) {}

void compressor::__build_key_tree() {
  unordered_map<uint8_t, __frequency_node> frequency;
  ifstream is(this->file_name, ios::binary | ios::in);
  priority_queue<__node_ptr, vector<__node_ptr>, less<__node_ptr>> q;

  __frequency_node n;
  while (is >> n) {
    if (frequency.count(n.__value) == 0) {
      frequency[n.__value].__value = n.__value;
      frequency[n.__value].__frequency = 1;
    } else {
      frequency[n.__value].__frequency++;
    }
  }

  for (auto it = frequency.begin(); it != frequency.end(); it++) {
    cout << it->second << endl;
    q.push(&it->second);
  }

  is.close();

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

  cout << "size=" << q.size() << endl;

  __node_ptr root = q.top();
  q.pop();

  cout << "root=" << *root << endl;

  cout << "merged address=" << merged << endl;
  cout << "a address=" << a << endl;
  cout << "b address=" << b << endl;
}

}  // namespace huffman