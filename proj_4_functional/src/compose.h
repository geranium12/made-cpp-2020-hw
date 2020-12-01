#ifndef INC_4_SRC_COMPOSE_H_
#define INC_4_SRC_COMPOSE_H_

#include <type_traits>

template<class F, typename ...Other>
struct Composer {
 public:
  using return_type = F;

 private:
  F f;
  Composer<Other...> other;

 public:
  Composer(F func, Other ...other) : f(func), other(other...) {
      static_assert((std::is_same<typename Composer<Other...>::return_type::result_type,
          typename F::argument_type>::value), "Incompatible types");
  }

  template<class... Arg>
  decltype(auto) operator()(const Arg&... arg) {
      return f(other(arg...));
  }
};

template <class F, class G>
struct Composer<F, G> {
 public:
  using return_type = F;

 private:
  F f;
  G g;

 public:
  Composer(F f, G g) : f(f), g(g) {}

  template<class... Arg>
  decltype(auto) operator()(const Arg&... arg) {
      static_assert(std::is_same<typename G::result_type, typename F::argument_type>::value, "Incompatible types");
      return f(g(arg...));
  }
};

template <class F>
struct Composer<F> {
 public:
  using return_type = F;

 private:
  F f;

 public:
  Composer(F func) : f(func) {}

  template<class... Arg>
  decltype(auto) operator()(const Arg&... arg) {
      return f(arg...);
  }
};

template <class... F>
auto compose(F... f) {
    return Composer<F...>(f...);
}

#endif //INC_4_SRC_COMPOSE_H_
