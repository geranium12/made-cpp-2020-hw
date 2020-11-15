#pragma once
#include <iterator>
#include <xmemory>

namespace task {

template<class T, class Alloc = std::allocator<T>>
class list {

 private:

  struct Node;

  using allocator_traits = typename std::allocator_traits<Alloc>;
  using node_allocator_traits = typename allocator_traits::template rebind_traits<Node>;
  using node_allocator = typename node_allocator_traits::template rebind_alloc<Node>;

 public:

  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename allocator_traits::pointer;
  using const_pointer = typename allocator_traits::const_pointer;

  template <typename P, typename R>
  class iterator_base {
   private:
    Node* pointer_;

   public:
    friend class list;

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_base() = default;
    explicit iterator_base(Node* pointer): pointer_(pointer) {}
    iterator_base(const iterator_base<P, R>& other) = default;

    ~iterator_base () = default;

    iterator_base& operator= (iterator_base<P, R> &&other) noexcept = default;

    reference operator* () const {
      return pointer_->value;
    }

    pointer operator-> () {
      return &pointer_->value;
    }

    iterator_base<P, R>& operator++ () {
      pointer_ = pointer_->next;
      return *this;
    }

    iterator_base<P, R>& operator-- () {
      pointer_ = pointer_->prev;
      return *this;
    }

    iterator_base<P, R> operator++ (int) {
      iterator_base<P, R> t(*this);
      pointer_ = pointer_->next;
      return t;
    }

    iterator_base<P, R> operator-- (int) {
      iterator_base<P, R> t(*this);
      pointer_ = pointer_->prev;
      return t;
    }

    bool operator== (const iterator_base<P, R> &other) const {
      return pointer_ == other.pointer_;
    }

    bool operator!= (const iterator_base<P, R> &other) const {
      return pointer_ != other.pointer_;
    }
  };

  using iterator = iterator_base<pointer, reference>;
  using const_iterator = iterator_base<const_pointer, const_reference>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static_assert(
      std::is_same<typename Alloc::value_type, value_type>::value,
      "Allocator::value_type must be same type as value_type!"
  );

 private:

  void DefaultConstructorBase() {
    head_ = tail_ = nullptr;
    size_ = 0;
  }

 public:

  list() {
    DefaultConstructorBase();
  }

  ~list() {
    EmptyAndClearBuffer(node_allocator_, head_, tail_, size_);
  }

  explicit list(const Alloc& alloc) : allocator_(alloc), node_allocator_() {
    DefaultConstructorBase();
  }

 private:

  void CountAndValueConstructorBase(size_type count, const_reference value) {
    head_ = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
    Node* new_node = head_;
    Node* prev_node = nullptr;
    size_ = 0;
    while (size_ <= count) {
      node_allocator_traits::construct(node_allocator_, new_node, value, prev_node, nullptr);
      if (prev_node) {
        prev_node->next = new_node;
      }
      prev_node = new_node;
      new_node = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      ++size_;
    }
    --size_;
    prev_node->next = new_node;
    tail_ = new_node;
    node_allocator_traits::construct(node_allocator_, tail_, T(), prev_node, nullptr);
  }

 public:

  list(size_t count, const T& value, const Alloc& alloc = Alloc()) : allocator_(alloc), node_allocator_() {
    CountAndValueConstructorBase(count, value);
  }

 private:

  void CountConstructorBase(size_type count) {
    head_ = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
    Node* new_node = head_;
    Node* prev_node = nullptr;
    size_ = 0;
    while (size_ <= count) {
      node_allocator_traits::construct(node_allocator_, new_node);
      new_node->prev = prev_node;
      new_node->next = nullptr;
      if (prev_node) {
        prev_node->next = new_node;
      }
      prev_node = new_node;
      new_node = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      size_++;
    }
    --size_;
    prev_node->next = new_node;
    tail_ = new_node;
    node_allocator_traits::construct(node_allocator_, tail_);
    tail_->prev = prev_node;
    tail_->next = nullptr;
  }

 public:

  explicit list(size_t count, const Alloc& alloc = Alloc()) : allocator_(alloc), node_allocator_() {
    CountConstructorBase(count);
  }

 private:

  void CopyConstructorBase(const list& other) {
    head_ = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
    Node* new_node = head_;
    Node* prev_node = nullptr;
    size_ = 0;
    Node* cur_node = other.head_;
    while (size_ <= other.size_) {
      node_allocator_traits::construct(node_allocator_, new_node, cur_node->value, prev_node, nullptr);
      if (prev_node) {
        prev_node->next = new_node;
      }
      prev_node = new_node;
      new_node = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      ++size_;
    }
    --size_;
    prev_node->next = new_node;
    tail_ = new_node;
    node_allocator_traits::construct(node_allocator_, tail_);
    tail_->prev = prev_node;
    tail_->next = nullptr;
  }

 public:

  list(const list<T>& other) {
    if (!other.empty()) {
      CopyConstructorBase(other);
    } else {
      DefaultConstructorBase();
    }
  }

 private:

  void MoveConstructorBase(list&& other) {
    head_ = std::move(other.head_);
    tail_ = std::move(other.tail_);
    size_ = std::move(other.size_);

    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }

 public:

  list(list&& other) : allocator_(std::move(other.allocator_)), node_allocator_(std::move(other.node_allocator_)) {
    MoveConstructorBase(std::move(other));
  }

  list& operator=(const list& other) {
    if (this == &other) {
      return *this;
    }

    EmptyAndClearBuffer(node_allocator_, head_, tail_, size_);

    if (allocator_traits::propagate_on_container_copy_assignment::value) {
      allocator_ = other.allocator_;
      node_allocator_ = other.node_allocator_;
    }

    if (!other.empty()) {
      CopyConstructorBase(other);
    } else {
      DefaultConstructorBase();
    }

    return *this;
  }

  list& operator=(list&& other) {
    if (this == &other) {
      return *this;
    }

    EmptyAndClearBuffer(node_allocator_, head_, tail_, size_);

    allocator_ = std::forward<Alloc>(other.allocator_);
    node_allocator_ = std::forward<node_allocator>(other.node_allocator_);
    MoveConstructorBase(std::forward<list>(other));

    return *this;
  }

  Alloc get_allocator() const {
    return allocator_;
  }

  T& front() {
    return head_->value;
  }

  const T& front() const {
    return head_->value;
  }

  T& back() {
    Node* node = tail_->prev;
    return node->value;
  }

  const T& back() const {
    const Node* node = tail_->prev;
    return node->value;
  }

  iterator begin() const {
    return iterator(head_);
  }

  iterator end() const {
    return iterator(tail_);
  }

  const_iterator cbegin() const {
    return const_iterator(head_);
  }

  const_iterator cend() const {
    return const_iterator(tail_);
  }

  reverse_iterator rbegin() const {
    return reverse_iterator(end());
  }

  reverse_iterator rend() const {
    return reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const {
    return reverse_iterator(end());
  }

  const_reverse_iterator crend() const {
    return reverse_iterator(begin());
  }

  bool empty() const {
    return size_ == 0;
  }

  size_t size() const {
    return size_;
  }

  size_t max_size() const {
    return allocator_traits::max_size(node_allocator_);
  }

  void clear() {
    EmptyAndClearBuffer(node_allocator_, head_, tail_, size_);
  }

  iterator insert(iterator pos, const T& value) {
    return emplace(pos, std::forward<const T&>(value));
  }

  iterator insert(const_iterator pos, T&& value) {
    return emplace(pos, std::move(value));
  }

  iterator insert(iterator pos, size_t count, const T& value) {
    for (size_t i = 0; i < count; ++i) {
      emplace(pos, std::forward<const T&>(value));
    }
    return pos;
  }

  iterator erase(iterator pos) {
    iterator it_pos(pos.pointer_);
    iterator it_pos2(pos.pointer_);
    ++it_pos2;
    return erase(it_pos, it_pos2);
  }

  iterator erase(iterator first, iterator last) {
    Node* first_node = first.pointer_;
    Node* last_node = last.pointer_;
    bool is_head = (first == begin());
    bool is_tail = (last == end());

    Node* next;
    while (first_node != last_node) {
      next = first_node->next;
      if (first_node->prev) {
        first_node->prev->next = first_node->next;
      }
      if (first_node->next) {
        first_node->next->prev = first_node->prev;
      }
      node_allocator_traits::destroy(node_allocator_, first_node);
      node_allocator_traits::deallocate(node_allocator_, first_node, sizeof(Node));
      first_node = next;
      --size_;
    }

    if (is_head && is_tail) {
      node_allocator_traits::destroy(node_allocator_, first_node);
      node_allocator_traits::deallocate(node_allocator_, first_node, sizeof(Node));
      head_ = tail_ = nullptr;
    } else if (is_head) {
      head_ = first_node;
    }

    return iterator(first_node);
  }

  void push_back(const T& value) {
    emplace_back(value);
  }

  void push_back(T&& value) {
    emplace_back(std::move(value));
  }

  void pop_back() {
    if (!empty()) {
      erase(--end());
    }
  }

  void push_front(const T& value) {
    emplace_front(value);
  }

  void push_front(T&& value) {
    emplace_front(std::move(value));
  }

  void pop_front() {
    if (!empty()) {
      erase(begin());
    }
  }

  template<class... Args>
  iterator emplace(iterator pos, Args&& ... args) {
    if (pos == end()) {
      Node* prev = nullptr;
      if (!tail_) {
        tail_ = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      } else {
        prev = tail_->prev;
        node_allocator_traits::destroy(node_allocator_, tail_);
      }

      Node* new_tail = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      node_allocator_traits::construct(node_allocator_, new_tail, T(), tail_, nullptr);
      node_allocator_traits::construct(node_allocator_, tail_, std::forward<Args>(args)..., nullptr, new_tail);

      tail_->prev = prev;
      tail_->next = new_tail;

      if (!head_) {
        head_ = tail_;
      }

      tail_ = new_tail;
      ++size_;
      return --end();
    } else if (pos == begin()) {
      Node* new_head = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      node_allocator_traits::construct(node_allocator_, new_head, std::forward<Args>(args)..., nullptr, head_);
      if (head_) {
        head_->prev = new_head;
      }
      head_ = new_head;
      ++size_;
      return begin();
    } else {
      Node* pos_pointer = pos.pointer_;
      Node* new_node = node_allocator_traits::allocate(node_allocator_, sizeof(Node));
      Node* prev = nullptr;
      if (pos_pointer) {
        prev = pos_pointer->prev;
      }
      node_allocator_traits::construct(node_allocator_, new_node, std::forward<Args>(args)..., prev, pos_pointer);
      if (prev) {
        prev->next = new_node;
      }
      pos_pointer->prev = new_node;
      ++size_;
      return iterator(new_node);
    }
  }

  template<class... Args>
  void emplace_back(Args&& ... args) {
    emplace(end(), args...);
  }

  template<class... Args>
  void emplace_front(Args&& ... args) {
    emplace(begin(), args...);
  }


  void resize(size_t count) {
    size_type size = size_;
    if (count > size_) {
      for (size_t i = 0; i < count - size; ++i) {
        push_back(value_type());
      }
    } else if (count < size_) {
      for (size_t i = 0; i < size - count; ++i) {
        pop_back();
      }
    }
  }

  void swap(list& other) {
    std::swap(allocator_, other.allocator_);
    std::swap(node_allocator_, other.node_allocator_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

 private:

  void MoveNode(Node** dest_node, Node** source_node)
  {
    Node* new_node = *source_node;
    *source_node = new_node->next;
    new_node->next = *dest_node;
    *dest_node = new_node;
  }

 public:

  void merge(list& other) {
    if (this == &other) {
      return;
    }

    if (!other.head_) {
      return;
    }

    if (!head_) {
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        return;
    }

    Node* node1 = head_;
    Node* node2 = other.head_;

    Node* new_head = new Node();
    if (node1->value < node2->value) {
      new_head->value = node1->value;
      node1 = node1->next;
    } else {
      new_head->value = node2->value;
      node2 = node2->next;
    }

    Node* cur_node = new_head;

    while (node1 != tail_ && node2 != other.tail_) {
      if (node1->value < node2->value) {
        MoveNode(&(cur_node->next), &node1);
        cur_node->next->prev = cur_node;
      } else {
        MoveNode(&(cur_node->next), &node2);
        cur_node->next->prev = cur_node;
      }
      cur_node = cur_node->next;
    }

    while (node1 != tail_) {
      MoveNode(&(cur_node->next), &node1);
      cur_node->next->prev = cur_node;
      cur_node = cur_node->next;
    }

    while (node2 != other.tail_) {
      MoveNode(&(cur_node->next), &node2);
      cur_node->next->prev = cur_node;
      cur_node = cur_node->next;
    }

    node_allocator_traits::destroy(node_allocator_, node2);
    node_allocator_traits::deallocate(node_allocator_, node2, sizeof(Node));
    other.head_ = other.tail_ = nullptr;
    size_ += other.size_;
    other.size_ = 0;

    cur_node->next = tail_;
    tail_->prev = cur_node;
    head_ = new_head;

  }

  void splice(iterator pos, list& other) {
    if (!pos.pointer_->prev) {
      other.tail_->prev->next = head_;
      head_->prev = other.tail_->prev;
      head_ = other.head_;
    } else {
      Node* cur_node = pos.pointer_->prev;
      Node* next_node = cur_node->next;
      Node* node2 = other.head_;

      while (node2 != other.tail_) {
        MoveNode(&(cur_node->next), &node2);
        cur_node->next->prev = cur_node;
        cur_node = cur_node->next;
      }

      next_node->prev = cur_node;
      cur_node->next = next_node;
    }

    node_allocator_traits::destroy(node_allocator_, other.tail_);
    node_allocator_traits::deallocate(node_allocator_, other.tail_, sizeof(Node));
    other.head_ = other.tail_ = nullptr;
    size_ += other.size_;
    other.size_ = 0;
  }

  void remove(const T& value) {
    iterator it = begin();
    while (it != end()) {
      if (*it == value) {
        erase(it);
        return;
      }
      ++it;
    }
  }

  void reverse() {
    if (size_ < 2) {
      return;
    }

    Node* node = head_;
    Node* next,* prev = nullptr;
    while (node != tail_) {
      next = node->next;
      node->next = node->prev;
      node->prev = next;
      prev = node;
      node = next;
    }
    tail_->prev = head_;
    head_->next = tail_;
    head_ = prev;
    head_->prev = nullptr;
  }

  void unique() {
    if (size_ < 2) {
      return;
    }

    Node* saved_node,* cur_node = head_;
    T prev = head_->value;
    cur_node = cur_node->next;
    while (cur_node && cur_node != tail_) {
      saved_node = cur_node->prev;
      if (cur_node->value == prev) {
        erase(iterator(cur_node));
        cur_node = saved_node->next;
      } else {
        prev = cur_node->value;
        saved_node = cur_node;
        cur_node = cur_node->next;
      }

    }
  }

  void sort() {
    if (size_ < 2) {
      return;
    }

    Node* node_i = head_;
    Node* node_j;
    while (node_i != tail_) {
      node_j = node_i->prev;
      while (node_j && node_j->next && node_j->next != tail_ && node_j->value > node_j->next->value) {
        std::swap(node_j->value, node_j->next->value);
        node_j = node_j->prev;
      }
      node_i = node_i->next;
    }
  }

 private:

  struct Node {
    T value;
    Node* prev;
    Node* next;

    Node() = default;

    explicit Node(T value) : value(value) {}

    Node(T value, Node* prev, Node* next) : value(value), prev(prev), next(next) {}

    Node(const Node& other) : value(other.value), prev(other.prev), next(other.next) {}

    Node(Node&& other) noexcept
        : value(std::forward<T>(other.value)), prev(std::move(other.prev)), next(std::move(other.next)) {
      other.value = nullptr;
      other.prev = other.next = nullptr;
    }

    Node& operator=(const Node& other) {
      if (this == &other) {
        return *this;
      }

      value = other.value;
      prev = other.prev;
      next = other.next;

      return *this;
    }

    Node& operator=(Node&& other) noexcept {
      if (this == &other) {
        return *this;
      }

      value = std::forward<T>(other.value);
      prev = std::move(other.prev);
      next = std::move(other.next);

      other.value = nullptr;
      other.prev = other.next = nullptr;

      return *this;
    }

    ~Node() {
      prev = next = nullptr;
    }
  };

  allocator_type allocator_;
  node_allocator node_allocator_;
  Node* head_;
  Node* tail_;
  size_type size_;

  void EmptyAndClearBuffer(
      node_allocator& node_allocator,
      Node*& head,
      Node*& tail,
      size_type& size) noexcept {
    Node* curHead = head;
    Node* curNode = head;
    if (curNode != tail) {
      while (curNode != tail) {
        curHead = curHead->next;
        node_allocator_traits::destroy(node_allocator_, curNode);
        node_allocator_traits::deallocate(node_allocator_, curNode, sizeof(Node));
        curNode = curHead;
      }
      node_allocator_traits::destroy(node_allocator_, tail);
      node_allocator_traits::deallocate(node_allocator_, tail, sizeof(Node));
    }
    tail = head = nullptr;
    size = 0;
  }

  void CopyBuffer(
      node_allocator& node_allocator,
      Node* from_head,
      Node* from_tail,
      Node* to_head,
      Node* to_tail,
      size_type& size) {
    Node* cur_from_node = from_head;
    Node* prev_node = nullptr;
    while (cur_from_node) {
      node_allocator_traits::construct(node_allocator, to_tail, cur_from_node->value, prev_node, nullptr);
      prev_node->next = to_tail;
      prev_node = to_tail;
      cur_from_node = cur_from_node->next;
      to_tail = to_tail->next;
      ++size;
    }
  }

  void MoveBuffer(
      node_allocator& from_allocator,
      Node*& from_head,
      Node*& from_tail,
      const size_type from_size,
      node_allocator& to_allocator,
      Node*& to_head,
      Node*& to_tail,
      size_type& size) {
    Node* curFromNode = from_head;
    for (size_type i(0); i < from_size; ++i) {
      node_allocator_traits::construct(
          to_allocator,
          to_tail,
          std::move_if_noexcept(curFromNode));
      curFromNode = curFromNode->next;
      ++to_tail;
    }
    size = from_size;

    ClearBuffer(from_allocator, from_head, from_tail, from_size);
  }
};
}
