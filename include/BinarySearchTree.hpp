#include "BinaryTree.hpp"
#include <algorithm>

template <class Key, class Compare = std::less<Key>> requires std::predicate<Compare, const Key&, const Key&>
class BinarySearchTree {
public:
  using Node = BinaryTree<Key>::Node;
  using Iterator = BinaryTree<Key>::InOrderIterator;
  using ReverseIterator = BinaryTree<Key>::ReverseInOrderIterator;

  BinarySearchTree() = default;  
  explicit BinarySearchTree(Compare _comp) : comp(std::move(_comp)) {}
  BinarySearchTree(BinarySearchTree&& other) noexcept = default; 
  BinarySearchTree& operator=(BinarySearchTree&& other) noexcept = default;
  BinarySearchTree(const BinarySearchTree& other) : 
    comp{other.comp},
    root{other.root ? std::make_unique<Node>(*other.root) : nullptr} 
  {}
  BinarySearchTree& operator=(const BinarySearchTree& other) {
    if (this != &other)
      *this = BinarySearchTree(other);
    return *this;
  }

  inline void clear() {
    root.reset();
  }
  
  template <class Type> requires std::constructible_from<Key, Type>
  std::pair<Iterator, bool> insert(Type&& key) {
    auto current = &root;
    Node* parent = nullptr;

    while (*current) {
      parent = current->get();
      if (comp(key, parent->key)) {
        current = &parent->left;
      } else if (comp(parent->key, key)) {
        current = &parent->right;
      } else {
        return {Iterator{current->get()}, false};
      }
    }

    *current = std::make_unique<Node>(std::forward<Type>(key), parent);
    return {Iterator{current->get()}, true};
  }

  Iterator find(std::convertible_to<Key> auto&& key) const {
    Node* current = root.get();
    while (current) {
      if (!comp(current->key, key) && !comp(key, current->key))
        return Iterator(current);
      current = comp(key, current->key) ? current->left.get() : current->right.get();
    }

    return end();
  }

  Iterator erase(Iterator it) {
    const auto current = it.current;
    const auto succesor = (++it).current;
    
    auto moveIntoCurrent = [this, u = current](std::unique_ptr<Node>& v) {
      if (v)
        v->parent = u->parent;
      if (!u->parent)
        root = std::move(v);
      else if (u == u->parent->left.get())
        u->parent->left = std::move(v);
      else
        u->parent->right = std::move(v);
    };

    if (!current->left)
      moveIntoCurrent(current->right);
    else if (!current->right)
      moveIntoCurrent(current->left);
    else {
      if (succesor->parent == current) {
        current->left->parent = succesor;
        succesor->left = std::move(current->left);

        moveIntoCurrent(current->right);
      } else {
        current->left->parent = succesor;
        succesor->left = std::move(current->left);

        auto y = std::move(succesor->parent->left);

        if (y->right)
          y->right->parent = succesor->parent;
        succesor->parent->left = std::move(y->right);

        current->right->parent = y.get();
        y->right = std::move(current->right);

        moveIntoCurrent(y);
      }
    }
    return Iterator{succesor};
  }
  
  Iterator begin() const {
    if (!root)
      return end();
    Node* current = root.get();
    while (current->left)
      current = current->left.get();
    return Iterator{current};
  }

  /*TO DO*/
  Iterator end() const {
    return Iterator{nullptr};
  }

  bool checkAllRightHelper(const Node* node, bool left) const {
    if (!node)
      return true;
    if ((node->parent->*(left ? &Node::left : &Node::right)).get() != node)
      return false;
    return checkAllRightHelper(node->left.get(), true) && checkAllRightHelper(node->right.get(), false);
  }

  bool checkAllRight() const {
    if (!root)
      return true;
    if (root->parent)
      return false;
    return checkAllRightHelper(root->left.get(), true) && checkAllRightHelper(root->right.get(), false); 
  }

private:
  Compare comp;
  std::unique_ptr<Node> root;
};
