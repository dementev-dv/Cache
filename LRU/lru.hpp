#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T, typename KeyT = int>
class LRUCache {
  using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
  using MapIt = typename std::unordered_map<KeyT, ListIt>::iterator;
  
 public:
  LRUCache(const LRUCache&) = delete;
  LRUCache operator=(const LRUCache&) = delete;

  LRUCache(size_t sz) : size_(sz) { }

  bool full() const { return (cache_.size() == size_); }

  template <typename F>
  bool lookup_update(KeyT key, F slow_get_page) {
    MapIt hit = hash_.find(key);
    if (hit == hash_.end()) {
      if (full()) {
        hash_.erase(cache_.back().first);
        cache_.pop_back();
      }
      cache_.emplace_front(key, slow_get_page(key));
      hash_.emplace(key, cache_.begin());
      return false;
    }

    ListIt elt = hit->second;
    if (elt != cache_.begin())
      cache_.splice(cache_.begin(), cache_, elt, std::next(elt));
    return true;
  }
  
 private:
  size_t size_;
  std::list<std::pair<KeyT, T>> cache_;
  std::unordered_map<KeyT, ListIt> hash_;
};



} // namespace caches
