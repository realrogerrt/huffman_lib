#include "huffman/core.h"

namespace huffman {

bool operator<(const __frequency_node& a, const __frequency_node& b) {
  return a.__frequency < b.__frequency;
}

__frequency_node* __frequency_node::__merge(
    __frequency_node* other) {

  __frequency_node* n = new __frequency_node;
  n->__frequency = this->__frequency + other->__frequency;

  n->__left = this;
  n->__right = other;

  other->__parent = n;
  this->__parent = n;

  return n;
}

std::ostream& operator<<(std::ostream& os, const __frequency_node& n) {
  return os << "value=" << n.__value << " " << "frequency=" << n.__frequency;
}

std::ifstream& operator>>(std::ifstream& is, __frequency_node& n) {
  char data;
  is >> data;

  n.__value = static_cast<uint8_t>(data);
  n.__frequency = 1;

  return is;
}

compressor::compressor(const string& fn): file_name(fn) {}

void compressor::__build_key_tree() {
    unordered_map<uint8_t, __frequency_node> frequency;
    ifstream                                 is(this->file_name, ios::binary | ios::in);
    priority_queue<__node_ptr, vector<__node_ptr>, less<__node_ptr>> q;

    __frequency_node n;
    while (is >> n)
    {
        if (frequency.count(n.__value) == 0)
        {
            frequency[n.__value].__value     = n.__value;
            frequency[n.__value].__frequency = 1;
        }
        else
        {
            frequency[n.__value].__frequency++;
        }
    }

    for (auto it = frequency.begin(); it != frequency.end(); it++)
    {
        cout << it->second << endl;
        q.push(&it->second);
    }

    is.close();

    __node_ptr a = nullptr;
    __node_ptr b = nullptr;
    __node_ptr merged = nullptr;

    while (q.size() > 1)
    {
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

}