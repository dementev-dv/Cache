#pragma once

#include <list>
#include <unordered_map>
#include <iterator>

namespace caches {

template <typename T, typename KeyT = int>
class FIFOCache {
  using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
  using MapIt = typename std::unordered_map<KeyT, T>::iterator;

 public:
  FIFOCache(const FIFOCache&) = delete;
  FIFOCache operator=(const FIFOCache&) = delete;

  FIFOCache(size_t size) : size_(size) { }

  bool full() { return cache_.size() == size_; }

  template <typename F>
  bool lookup_update(KeyT key, F slow_get_page) {
    MapIt hit = hash_.find(key);
    if (hit != hash_.end()) {
      return true;
    }
    if (full()) {
      hash_.erase(cache_.back().first);
      cache_.pop_back();
    }
    T elt = slow_get_page(key);
    cache_.emplace_front(key, elt);
    hash_.emplace(key, elt);
    return false;
  }

 private:
  size_t size_;
  std::list<std::pair<KeyT, T>> cache_;
  std::unordered_map<KeyT, T> hash_;
};


}; // namespace caches
