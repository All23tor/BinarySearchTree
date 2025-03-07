#include <memory>
#include <type_traits>

template <class T>
struct BinaryTree {
  struct Node {
    T key;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node* parent;

    Node() = delete;
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;
    Node(const Node& other, Node* _parent = nullptr): 
      key{other.key}, parent{_parent},
      left{other.left ? std::make_unique<Node>(*other.left, this) : nullptr},
      right{other. right ? std::make_unique<Node>(*other.right, this) : nullptr}
    {}
    Node& operator=(const Node& other) {
      if (this != &other)
        *this = Node(other);
      return *this;
    }

    template <class _T> requires std::is_same_v<std::remove_cvref_t<_T>, T>
    explicit Node(_T&& value, Node* _parent = nullptr):
      key{std::forward<_T>(value)},
      parent{_parent}
    {}
  };

  struct InOrderIterator {
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    using Node = BinaryTree<T>::Node;
    Node* current;

    reference operator*() const {
      return current->key;
    }

    pointer operator->() const {
      return &current->key;
    }

    bool operator==(const InOrderIterator& o) const {
      return this->current == o.current;
    }

    bool operator!=(const InOrderIterator& o) const {
      return !operator==(o);
    }

    InOrderIterator& operator++() {
      if (current->right) {
        current = current->right.get();
        while (current->left)
          current = current->left.get();
        return *this;
      }
      
      bool wasRightChild;
      do {
        wasRightChild = (current->parent && current == current->parent->right.get());
        current = current->parent;
      } while (wasRightChild);
      return *this;
    }
    
    InOrderIterator operator++(int) {
      InOrderIterator it = *this;
      ++*this;
      return it;
    }

    InOrderIterator& operator--() {
      if (current->left) {
        current = current->left.get();
        while (current->right)
          current = current->right.get();
        return *this;
      }
      
      bool wasLeftChild;
      do {
        wasLeftChild = (current->parent && current == current->parent->left.get());
        current = current->parent;
      } while (wasLeftChild);
      return *this;
    }


    InOrderIterator operator--(int) {
      InOrderIterator it = *this;
      --*this;
      return it;
    }
  };

  using ReverseInOrderIterator = std::reverse_iterator<InOrderIterator>;

  std::unique_ptr<Node> root;
};

