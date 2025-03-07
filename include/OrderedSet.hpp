#include "BinarySearchTree.hpp"

template <class Key, class Compare = std::less<Key>> requires std::predicate<Compare, const Key&, const Key&>
class OrderedSet {
  std::size_t amountOfNodes = 0;
  BinarySearchTree<Key, Compare> bst;

public:
  OrderedSet() = default;
  OrderedSet(const OrderedSet&) = default;
  OrderedSet(OrderedSet&& other): amountOfNodes{std::move(other.amountOfNodes)}, bst{std::move(other.bst)} {
    other.amountOfNodes = 0;
  };
  OrderedSet& operator=(const OrderedSet&) = default;
  OrderedSet& operator=(OrderedSet&&) = default;
  ~OrderedSet() = default;
  explicit OrderedSet(Compare _comp): bst{std::move(_comp)} {}
  template <std::forward_iterator InputIt>
  OrderedSet(InputIt first, InputIt last, Compare _comp = Compare()) : bst{std::move(_comp)} {
    insert(first, last);
  }
  OrderedSet(std::initializer_list<Key> initList, Compare _comp = Compare()) : bst{std::move(_comp)} {
    insert(initList.begin(), initList.end());
  }
  OrderedSet& operator=(std::initializer_list<Key> initList) {
    return *this = OrderedSet(initList);
  }
  
  using Iterator = BinarySearchTree<Key>::Iterator;
  
  template <class Type> requires std::constructible_from<Key, Type>
  std::pair<Iterator, bool> insert(Type&& key) {
    auto ret = bst.insert(std::forward<Type>(key));
    if (ret.second)
      amountOfNodes++;
    return ret;
  }
  
  template< class... Args >
  std::pair<Iterator, bool> emplace(Args&&... args) {
    insert(Key(std::forward<Args>(args)...));
  }
  
  template <std::input_iterator InputIt>
    requires std::convertible_to<std::iter_value_t<InputIt>, Key>
  void insert(InputIt first, InputIt last) {
    for (auto it = first; it != last; ++it) {
      insert(*it);
    }
  }
  
  void insert(std::initializer_list<Key> initList) {
    for (const auto& e: initList) {
      insert(e);
    }
  }
  
  Iterator find(std::convertible_to<Key> auto&& key) const {
    return bst.find(key);
  }
  
  Iterator erase(const Iterator it) {
    amountOfNodes--; 
    return bst.erase(it);
  }

  Iterator begin() const {
    return bst.begin();
  }
  
  Iterator end() const {
    return bst.end();
  }

  void swap(OrderedSet& other) noexcept {
    std::swap(*this, other);
  }

  void clear() {
    bst.clear();
    amountOfNodes = 0;
  }

  bool empty() const {
    return !amountOfNodes;
  }

  std::size_t size() const {
    return amountOfNodes;
  }

  std::size_t count(const Key& key) const {
    if (contains(key))
      return 1;
    return 0;
  }
  
  bool contains(const Key& value) const {
    return bst.find(value) != bst.end();
  }

  bool checkAllRight() const {
    return bst.checkAllRight();
  }

  /*TO DO*/
  Iterator lowerBound(const Key& key) const;
  Iterator upperBound(const Key& key) const;
};

template<class Key, class Compare>
bool operator==(const OrderedSet<Key, Compare>& lhs, const OrderedSet<Key, Compare>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class Key, class Compare>
auto operator<=>(const OrderedSet<Key, Compare>& lhs, const OrderedSet<Key, Compare>& rhs) {
  return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
