#include "smart_pointers.h"

namespace task {

template<class T>
UniquePtr<T>::UniquePtr(T* ptr) noexcept : pointer_(ptr) {}

template<class T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& other) noexcept : pointer_(std::move(other.pointer_)) {
  other.pointer_ = nullptr;
}

template<class T>
UniquePtr<T>::~UniquePtr() {
  delete pointer_;
}

template<class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  pointer_ = std::move(other.pointer_);
  other.pointer_ = nullptr;
}

template<class T>
T& UniquePtr<T>::operator*() const noexcept {
  return *pointer_;
}

template<class T>
T* UniquePtr<T>::operator->() const noexcept {
  return pointer_;
}

template<class T>
T* UniquePtr<T>::get() const noexcept {
  return pointer_;
}

template<class T>
T* UniquePtr<T>::release() noexcept {
  T* result = nullptr;
  std::swap(result, pointer_);
  return result;
}

template<class T>
void UniquePtr<T>::reset(T* ptr) noexcept {
  T* old_ptr = release();
  delete old_ptr;
}

template<class T>
void UniquePtr<T>::swap(UniquePtr<T>& other) noexcept {
  std::swap(pointer_, other.pointer_);
}


template<class T>
ControlBlock<T>::ControlBlock() noexcept : pointer_(nullptr), shared_counter_(0) {}

template<class T>
ControlBlock<T>::ControlBlock(T* ptr) noexcept : pointer_(ptr), shared_counter_(1) {}

template<class T>
void ControlBlock<T>::del() noexcept {
  if (shared_counter_ == 1) {
    delete pointer_;
  }
  --shared_counter_;
}


template<class T>
void SharedPtr<T>::destroy() {
  if (control_block_) {
    control_block_->del();
  }
}

template<class T>
constexpr SharedPtr<T>::SharedPtr() noexcept : control_block_(nullptr) {}

template<class T>
SharedPtr<T>::SharedPtr(T* pointer) : control_block_(new ControlBlock(pointer)) {}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) noexcept : control_block_(other.control_block_) {
  if (control_block_) {
    ++control_block_->shared_counter_;
  }
}

template<class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept : control_block_(std::move_if_noexcept(other.control_block_)) {
  other.control_block_ = nullptr;
}

template<class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& weak_ptr) : control_block_(weak_ptr.control_block_) {
  if (control_block_) {
    ++control_block_->shared_counter_;
  }
}

template<class T>
SharedPtr<T>::~SharedPtr() {
  destroy();
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) noexcept {
  if (this == &other) {
    return *this;
  }

  destroy();

  control_block_ = other.control_block_;
  if (control_block_) {
    ++control_block_->shared_counter_;
  }

  return *this;
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  destroy();

  control_block_ = std::move_if_noexcept(other.control_block_);
  other.control_block_ = nullptr;

  return *this;
}

template<class T>
T& SharedPtr<T>::operator*() const noexcept {
  return *get();
}

template<class T>
T* SharedPtr<T>::operator->() const noexcept {
  return get();
}

template<class T>
T* SharedPtr<T>::get() const noexcept {
  return (control_block_ ? control_block_->pointer_ : nullptr);
}

template<class T>
long SharedPtr<T>::use_count() const noexcept {
  return (control_block_ ? control_block_->shared_counter_ : 0);
}

template<class T>
void SharedPtr<T>::reset() noexcept {
  destroy();
  control_block_ = nullptr;
}

template<class T>
void SharedPtr<T>::reset(T* ptr) {
  destroy();
  control_block_ = new ControlBlock(ptr);
}

template<class T>
void SharedPtr<T>::swap(SharedPtr<T>& other) noexcept {
  std::swap(control_block_, other.control_block_);
}


template<class T>
constexpr WeakPtr<T>::WeakPtr() noexcept : control_block_() {}

template<class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared_ptr) noexcept : control_block_(shared_ptr.control_block_) {}

template<class T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) noexcept : control_block_(other.control_block_) {}

template<class T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) noexcept : control_block_(std::move_if_noexcept(other.control_block_)) {
  other.control_block_ = nullptr;
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other) noexcept {
  if (this == other) {
    return *this;
  }

  if (control_block_) {
    control_block_->del();
  }
  control_block_ = other.control_block_;

  return *this;
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) noexcept {
  if (this == other) {
    return *this;
  }

  control_block_ = std::move_if_noexcept(other.control_block_);
  other.control_block_ = nullptr;

  return *this;
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other) noexcept {
  control_block_ = other.control_block_;

  return *this;
}

template<class T>
bool WeakPtr<T>::expired() const noexcept {
  return use_count() == 0;
}

template<class T>
long WeakPtr<T>::use_count() const noexcept {
  if (control_block_) {
    return control_block_->shared_counter_;
  }
  return 0;
}

template<class T>
SharedPtr<T> WeakPtr<T>::lock() const noexcept {
  return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
}

template<class T>
void WeakPtr<T>::reset() noexcept {
  control_block_ = nullptr;
}

template<class T>
void WeakPtr<T>::swap(const WeakPtr<T>& other) noexcept {
  std::swap(control_block_, other.control_block_);
}
}