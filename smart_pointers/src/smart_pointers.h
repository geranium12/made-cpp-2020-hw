#pragma once


#include <algorithm>
#include <utility>
#include <memory>

namespace task {


template <class T>
class UniquePtr;

template <class T>
class SharedPtr;

template <class T>
class WeakPtr;

template <class T>
class UniquePtr {
 private:
  T* pointer_;

 public:

  explicit UniquePtr(T* ptr) noexcept;
  UniquePtr(const UniquePtr<T>&) = delete;
  UniquePtr(UniquePtr<T>&& other) noexcept;

  ~UniquePtr();

  UniquePtr& operator=(const UniquePtr<T>&) = delete;
  UniquePtr& operator=(UniquePtr<T>&& other) noexcept;

  T& operator*() const noexcept;
  T* operator->() const noexcept;
  T* get() const noexcept;

  T* release() noexcept;
  void reset(T* ptr = T()) noexcept;
  void swap(UniquePtr<T>& other) noexcept;
};

template <class T>
struct ControlBlock {
  T* pointer_;
  long shared_counter_;

  explicit ControlBlock() noexcept;
  explicit ControlBlock(T* ptr) noexcept;

  ~ControlBlock() = default;

  void del() noexcept;
};

template <class T>
class SharedPtr {
 private:
  ControlBlock<T>* control_block_;

  friend class WeakPtr<T>;

  void destroy();

 public:

  constexpr SharedPtr() noexcept;
  explicit SharedPtr(T* pointer);
  SharedPtr(const SharedPtr<T>& other) noexcept;
  SharedPtr(SharedPtr<T>&& other) noexcept;
  explicit SharedPtr(const WeakPtr<T>& weak_ptr);

  ~SharedPtr();

  SharedPtr& operator=(const SharedPtr<T>& other) noexcept;
  SharedPtr& operator=(SharedPtr<T>&& other) noexcept;

  T& operator*() const noexcept;
  T* operator->() const noexcept;
  T* get() const noexcept;

  long use_count() const noexcept;

  void reset() noexcept;
  void reset(T* ptr);
  void swap(SharedPtr<T>& other) noexcept;
};

template <class T>
class WeakPtr {
 private:
  ControlBlock<T>* control_block_;

  friend class SharedPtr<T>;

 public:
  constexpr WeakPtr() noexcept;
  WeakPtr(const SharedPtr<T>& shared_ptr) noexcept;
  WeakPtr(const WeakPtr<T>& other) noexcept;
  WeakPtr(WeakPtr<T>&& other) noexcept;

  ~WeakPtr() = default;

  WeakPtr& operator=(const WeakPtr<T>& other) noexcept;
  WeakPtr& operator=(WeakPtr<T>&& other) noexcept;
  WeakPtr& operator=(const SharedPtr<T>& other) noexcept;

  bool expired() const noexcept;
  long use_count() const noexcept;

  SharedPtr<T> lock() const noexcept;

  void reset() noexcept;
  void swap(const WeakPtr<T>& other) noexcept;
};


// Your template function definitions may go here...

}  // namespace task

#include "smart_pointers.tpp"