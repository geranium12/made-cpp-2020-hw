#include <iostream>
#include <vector>

template <typename T>
struct Allocator {
 private:
  static const std::size_t CHUNK_SIZE = 1 << 7;

  struct Chunk {
    std::size_t size;
    std::size_t allocators_counter;
    T* buffer;
    Chunk* prev;

    explicit Chunk(std::size_t allocators_counter) : allocators_counter(allocators_counter) {
      size = 0;
      buffer = new T[CHUNK_SIZE];
      prev = nullptr;
    }

    ~Chunk() {
      delete[] buffer;
    }
  };

  Chunk* head;

  T* allocateChunk(std::size_t n, std::size_t allocators_counter) {
    Chunk* newChunk = new Chunk(allocators_counter);
    newChunk->size = n;
    newChunk->prev = head;
    head = newChunk;
    return head->buffer;
  }

  T* allocateBlock(std::size_t n) {
    if (n > CHUNK_SIZE - head->size) {
      Chunk* curChunk = head;
      while (curChunk) {
        if (n <= CHUNK_SIZE - curChunk->size) {
          break;
        }
        curChunk = curChunk->prev;
      }
      if (!curChunk) {
        return allocateChunk(n, head->allocators_counter);
      }

      std::size_t prevSize = curChunk->size;
      curChunk->size += n;
      return curChunk->buffer + prevSize;
    } else {
      std::size_t prevSize = head->size;
      head->size += n;
      return head->buffer + prevSize;
    }
  }

  void copy(const Allocator<T>& other) {
    head = other.head;
    Chunk* chunk = head;
    while (chunk) {
      ++(chunk->allocators_counter);
      chunk = chunk->prev;
    }
  }

  void destroy() {
    Chunk* curChunk = head;
    Chunk* temp = head;
    while (curChunk) {
      curChunk = curChunk->prev;
      if (temp->allocators_counter == 1) {
        delete temp;
      } else {
        --temp->allocators_counter;
      }
      temp = curChunk;
    }
  }

 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template<typename U>
  struct rebind {
    typedef Allocator<U> other;
  };

  Allocator() noexcept : head(nullptr) {}

  template <class U>
  constexpr Allocator(const Allocator<U>& other) noexcept {
    copy(other);
  }

  Allocator<T>& operator=(const Allocator<T>& other) {
    if (this != &other) {
      destroy();
      copy();
    }

    return *this;
  }

  ~Allocator() {
    destroy();
  }

  pointer allocate(std::size_t n) {
    if (n > CHUNK_SIZE || n < 1) {
      throw std::bad_alloc();
    }

    if (head) {
      return allocateBlock(n);
    }

    return allocateChunk(n, 1);
  }

  void deallocate(T* p, std::size_t n) noexcept {}

  template<typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    ::new((void* )p) U(std::forward<Args>(args)...);
  }

  template<typename U>
  void destroy(U* p) {
    p->~U();
  }
};

class A {
 public:
  A() {}

  A(int x, int y) {
    std::cout << "A(" << x << ", " << y << ")\n";
  }

  ~A() {}
};

int main() {
  Allocator<int> allocator;

  // Test 1
  int* arr4 = allocator.allocate(4);

  allocator.construct(arr4, 49);
  allocator.construct(arr4 + 1, 48);
  allocator.construct(arr4 + 2, 47);
  allocator.construct(arr4 + 3, 46);

  for (int i = 0; i < 4; ++i) {
    std::cout << arr4[i] << std::endl;
  }

  // Test 2
  int* arr5 = allocator.allocate(5);

  allocator.construct(arr5, 51);
  allocator.construct(arr5 + 1, 52);
  allocator.construct(arr5 + 2, 53);
  allocator.construct(arr5 + 3, 54);
  allocator.construct(arr5 + 4, 55);

  for (int i = 0; i < 5; ++i) {
    std::cout << arr5[i] << std::endl;
  }

  int* arr2 = allocator.allocate(2);
  allocator.construct(arr2, 223);
  allocator.construct(arr2 + 1, 224);

  for (int i = 0; i < 2; ++i) {
    std::cout << arr2[i] << std::endl;
  }

  allocator.destroy(arr2);
  allocator.destroy(arr2 + 1);
  allocator.deallocate(arr2, 2);

  // Test 3
  Allocator<int> allocator2(allocator);
  Allocator<int> allocator3 = allocator2;

  // Test 4
  Allocator<int> a1;
  int* a = a1.allocate(1);
  a1.construct(a, 7);
  std::cout << a[0] << '\n';
  a1.deallocate(a, 1);

  Allocator<std::string> a2;

  decltype(a1)::rebind<std::string>::other a2_1;

  std::allocator_traits<decltype(a1)>::rebind_alloc<std::string> a2_2;

  std::string* s = a2.allocate(2);

  a2.construct(s, "foo");
  a2.construct(s + 1, "bar");

  std::cout << s[0] << ' ' << s[1] << '\n';

  a2.destroy(s);
  a2.destroy(s + 1);
  a2.deallocate(s, 2);

  // Test 5
  Allocator<A> allocatorA;
  A* arrA = allocatorA.allocate(3);

  allocatorA.construct(arrA, A(1, 1));
  allocatorA.construct(arrA + 1, A(2, 2));
  allocatorA.construct(arrA + 2, A(3, 3));

  allocatorA.destroy(arrA);
  allocatorA.destroy(arrA + 1);
  allocatorA.destroy(arrA + 2);
  allocatorA.deallocate(arrA, 3);

  return 0;
}