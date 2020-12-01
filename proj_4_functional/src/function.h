#include <iostream>
#include <memory>
#include <typeinfo>


template <class>
class function;

template <class R, class... Args>
class function <R (Args...)> {
 public:
  using return_type = R;

 private:
  class callable_base {
   private:
    callable_base(const callable_base& );
    void operator=(const callable_base&);

   public:
    callable_base() {}
    virtual ~callable_base() {}

    virtual R invoke(Args... args) = 0;
    virtual std::unique_ptr<callable_base> copy() = 0;
    virtual const std::type_info& getTargetType() = 0;
  };

  template <typename FunctionType>
  class callable_function: public callable_base {
   private:
    FunctionType functor;

   public:
    callable_function(FunctionType func): callable_base(), functor(func) {}

    virtual R invoke(Args... args) {
        return functor(args...);
    }


    virtual std::unique_ptr<callable_base> copy() {
        return std::unique_ptr<callable_base>(new callable_function(functor));
    }

    virtual const std::type_info& getTargetType() override {
        return typeid(FunctionType);
    }
  };

  template <typename FunctionType, typename ClassType, typename ... ArgumentTypes>
  class callable_class_member: public callable_base {
   private:
    FunctionType ClassType::* functor;

   public:

    callable_class_member(FunctionType ClassType::* f): functor(f) {}

    virtual R invoke(ClassType obj, ArgumentTypes... arguments) {
        return (obj.*functor)(arguments...);
    }

    virtual std::unique_ptr<callable_base> copy() {
        return std::unique_ptr<callable_base>(new callable_class_member(functor));
    }

    virtual const std::type_info& getTargetType() override {
        return typeid(FunctionType ClassType::*);
    }
  };


  std::unique_ptr<callable_base> f_ptr;

 public:
  function() noexcept: f_ptr(nullptr) {}

  function(nullptr_t) noexcept : f_ptr(nullptr) {}

  function(const function& other): f_ptr(other.f_ptr->copy()) {}

  function(function&& other): f_ptr(std::move(other.f_ptr)) {}

  template <typename F>
  function(F f): f_ptr(new callable_function<F>(f)) {}

  template <typename FunctionType, typename ClassType>
  function(FunctionType ClassType::* f): f_ptr(new callable_class_member<FunctionType, Args...>(f)) {}


  ~function() {
      f_ptr.reset(nullptr);
  }


  function& operator=(const function& other) {
      if (this == &other) {
          return *this;
      }

      function(other).swap(*this);
      return *this;
  }

  function& operator=(function&& other) {
      if (this == &other) {
          return *this;
      }

      function(std::move(other)).swap(*this);
      return *this;
  }

  function& operator=(std::nullptr_t) noexcept {
      f_ptr.release();
      return *this;
  }

  template <class F>
  function& operator=(F&& f) {
      function(std::forward<F>(f)).swap(*this);
      return *this;
  }

  template <class F>
  function& operator=(std::reference_wrapper<F> f) noexcept {
      function(f).swap(*this);
      return *this;
  }

  void swap(function& other) noexcept {
      if (this == &other) {
          return;
      }

      f_ptr.swap(other.f_ptr);
  }

  explicit operator bool() const noexcept {
      return f_ptr.operator bool();
  }

  R operator ()(Args... args) {
      if (!*this == true) {
          throw std::bad_function_call();
      }
      return f_ptr->invoke(args...);
  }

  const std::type_info& target_type() const noexcept {
      if (this) {
          return f_ptr->getTargetType();
      } else {
          return typeid(void);
      }
  }
};

template <class R, class... Args>
void swap(function<R(Args...)>& lhs,
          function<R(Args...)>& rhs ) noexcept {
    lhs.swap(rhs);
}

template <class R, class... ArgTypes>
bool operator==(const function<R(ArgTypes...)>& f,
                std::nullptr_t) noexcept {
    return !f;
}

template <class R, class... ArgTypes>
bool operator==(std::nullptr_t,
                const function<R(ArgTypes...)>& f) noexcept {
    return !f;
}

template <class R, class... ArgTypes>
bool operator!=(const function<R(ArgTypes...)>& f,
                std::nullptr_t) noexcept {
    return (bool) f;
}

template <class R, class... ArgTypes>
bool operator!=(std::nullptr_t,
                const function<R(ArgTypes...)>& f) noexcept {
    return (bool) f;
}