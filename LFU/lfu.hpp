#pragma once

#include <list>
#include <unordered_map>
#include <iterator>

namespace caches {

template <typename T, typename KeyT = int>
class LFUCache {
  struct Item {
    T val;
    int freq;
    Item(T v, int f) : val(v), freq(f) { }
  };

  using CacheIt = typename std::unordered_map<KeyT, Item>::iterator;

 public:
  LFUCache(const LFUCache&) = delete;
  LFUCache operator=(const LFUCache&) = delete;

  LFUCache(size_t size) : size_(size) { }

  bool full() { return cache_.size() == size_; }

  template <typename F>
  bool lookup_update(KeyT key, F slow_get_page) {
    CacheIt it = cache_.find(key);
    if (freq_.find(key) == freq_.end())
      freq_[key] = 1;
    else
      freq_[key]++;

    if (it != cache_.end()) {
      it->second.freq++;
      return true;
    }
    
    if (full()) {
      it = cache_.begin();
      for (CacheIt curr = cache_.begin(); curr != cache_.end(); curr++) {
        if (curr->second.freq < it->second.freq)
          it = curr;
      }
      cache_.erase(it->first);
    }

    cache_.emplace(key, Item(slow_get_page(key), freq_[key]));      // move constructor needed??
    return false;
  }

 private:
  size_t size_;
  std::unordered_map<KeyT, Item> cache_;
  std::unordered_map<KeyT, int> freq_; 
};


}; // namespace caches
