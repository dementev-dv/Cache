#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>

namespace caches {

template <typename KeyT = int>
class BeladyCache {
 public:
  Belady(const Belady&) = delete;
  Belady operator=(const Belady&) = delete;

  Belady(size_t size, std::vector<KeyT>& keys)
    : size_(size),
      keys_(keys) { }

  bool hit(int idx) {
    for (int i = 0; i < cache_.size(); i++) {
      if (cache_[i] == keys_[idx])
        return true;
    }
    return false;
  }

  int count_hits() {
    int hits = 0;
    for (int idx = 0; idx < keys_.size(); idx++) {
      // check for hit
      if (hit(idx)) {
        hits++;
        continue;
      }

      // check if key can be placed to cache
      if (cache_.size() < size_) {
        cache_.push_back(keys_[idx]);
        continue;
      }

      // count replaced element
      int far = idx;
      int replace = -1;
      for (int i = 0; i < cache_.size(); i++) {
        int j;
        for (j = idx + 1; j < keys_.size(); j++) {
          if (cache_[i] == keys_[j] && j > far) {
            far = j;
            replace = i;
            break;
          }
        }
        if (j == keys_.size()) {
          replace = i;
          break;
        }
      }
      
      assert(replace >= 0);
      cache_[replace] = keys_[idx];
    }

    return hits;
  }

 private:
  size_t size_;
  std::vector<KeyT> cache_;
  std::vector<KeyT>& keys_;
};

};
