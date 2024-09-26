#include <unordered_map>
#include <list>
#include <iterator>

namespace caches {                                                              // same namespace??

template <typename T, typename KeyT = int>
class Q2Cache {
  using CListIt = typename std::list<std::pair<KeyT, T>>::iterator;
  using CMapIt = typename std::unordered_map<KeyT, CListIt>::iterator;
  using OutListIt = typename std::list<KeyT>::iterator;
  using OutMapIt = typename std::unordered_map<KeyT, OutListIt>::iterator;

 public:
  Q2Cache(size_t sz) 
  : main_sz_(sz * 2 / 3),
    in_sz_(sz / 3 + sz % 3),
    out_sz_(in_sz_) {
      if(sz < 3)  {
        std::cout << "Q2 Cache size was automatically set to 3" << std::endl;   // static feature??
        main_sz_= 2;
        in_sz_ = 1;
        out_sz_ = 1;
      }
  }

  Q2Cache& operator=(const Q2Cache&) = delete;
  Q2Cache(const Q2Cache&) = delete;

  bool main_full() { return (main_.size() == main_sz_); }
  bool in_full() { return (in_.size() == in_sz_); }
  bool out_full() { return (out_.size() == out_sz_); }

  template<typename F>
  bool lookup_update(KeyT key, F slow_get_page) {
    CMapIt hit = main_hash_.find(key);
    // std::cout << "looking for " << key << " in main" << std::endl;
    if (hit != main_hash_.end()) {
      // std::cout << "found " << key << " in main" << std::endl;
      CListIt elt = hit->second;
      if (elt != main_.begin())
        main_.splice(main_.begin(), main_, elt, std::next(elt));
      return true;
    }

    // std::cout << "looking for " << key << " in out" << std::endl;
    OutMapIt outhit = out_hash_.find(key);                                      // decl here??
    if (outhit != out_hash_.end()) {
      // std::cout << "found " << key << " in out" << std::endl;
      if (main_full()) {
        main_hash_.erase(main_.back().first);
        main_.pop_back();
      }
      main_.emplace_front(key, slow_get_page(key));
      main_hash_.emplace(key, main_.begin());
      return false;
    }

    // std::cout << "looking for " << key << " in in" << std::endl;
    hit = in_hash_.find(key);
    if (hit != in_hash_.end()) {
      // std::cout << "found " << key << " in in" << std::endl;
      return true;
    }
    if (in_full()) {
      KeyT moved = in_.back().first;
      // std::cout << "moving " << moved << " from in to out" << std::endl;
      in_hash_.erase(moved);
      in_.pop_back();
      if (out_full()) {
        out_hash_.erase(out_.back());
        out_.pop_back();
      }
      out_.emplace_front(moved);
      out_hash_.emplace(moved, out_.begin());
    }
    in_.emplace_front(key, slow_get_page(key));
    in_hash_.emplace(key, in_.begin());

    // std::cout << "not found " << key << " in cache" << std::endl;
    return false;
  }

 private:
  std::list<std::pair<KeyT, T>> main_;                                          // class Buffer??
  std::list<std::pair<KeyT, T>> in_;
  std::list<KeyT> out_;

  std::unordered_map<KeyT, CListIt> main_hash_;
  std::unordered_map<KeyT, CListIt> in_hash_;
  std::unordered_map<KeyT, OutListIt> out_hash_;

  size_t main_sz_;
  size_t in_sz_;
  size_t out_sz_;
};


} // namespace caches
