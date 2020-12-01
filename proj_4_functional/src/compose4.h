#ifndef INC_4_SRC_COMPOSE4_H_
#define INC_4_SRC_COMPOSE4_H_
#include <iostream>

/*
 *
 * template<typename T>
class DetectX
{
  struct Fallback { int X; }; // add member name "X"
  struct Derived : T, Fallback { };

  template<typename U, U> struct Check;

  typedef char ArrayOfOne[1];  // typedef for an array of size one.
  typedef char ArrayOfTwo[2];  // typedef for an array of size two.

  template<typename U>
  static ArrayOfOne & f(Check<int Fallback::*, &U::argument_type> *);

  template<typename U>
  static ArrayOfTwo & f(...);

 public:
  typedef DetectX type;
  enum { value = sizeof(f<Derived>(0)) == 2 };
};


template <typename F0, typename... F>
class Composer {
  F0 f0_;
  Composer<F...> tail_;
 public:
  Composer(F0 f0, F... f) : f0_(f0), tail_(f...) {}

  template <typename T>
  decltype(auto) operator() (const T& x) const {
      return f0_(tail_(x));
  }
};

template <typename F, typename G>
class Composer<F, G> {
  F f_;
  G g_;
 public:
  Composer(F f, G g) : f_(f), g_(g) {}

  template <typename T>
  decltype(auto) operator() (const T& x) const {
      std::cout << DetectX<F>::value << std::endl;
      //std::cout << F::argument_type() << std::endl;
      if (DetectX<F>::value == true) {
          static constexpr bool value = std::is_same<typename G::result_type, typename F::argument_type>::value;
          static_assert((value == true), "Incompatible types in functions!");
      } else {
          std::cout << "DGHFFFFF" << std::endl;
          std::function<std::remove_reference<F>::type> f(f_);
          static constexpr bool value = std::is_same<typename G::result_type, typename std::function<std::remove_reference<F>::type>::argument_type>::value;
          static_assert((value == true), "Incompatible types in functions!");
      }
      return f_(g_(x));
  }
};

template <typename F>
class Composer<F> {
  F f_;
 public:
  Composer(F f) : f_(f) {}

  template <typename T>
  decltype(auto) operator() (const T& x) const {
      return f_(x);
  }
};

template <typename... F>
Composer<F...> compose(F... f) {
    return Composer<F...>(f...);
}

*/



#endif //INC_4_SRC_COMPOSE4_H_
