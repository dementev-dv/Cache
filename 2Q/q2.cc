#include <iostream>

#include "q2.hpp"

int slow_get_page_int(int key) { return key; }

int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;

  caches::Q2Cache<int> c{m};

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    if (c.lookup_update(q, slow_get_page_int))
    hits ++;
  }

  std::cout << hits << std::endl;
}