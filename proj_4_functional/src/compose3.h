#ifndef INC_4_SRC_COMPOSE3_H_
#define INC_4_SRC_COMPOSE3_H_

template <typename T>
class has_argument_type {
  typedef char yes_type;
  typedef long no_type;
  template <typename U> static yes_type test(decltype(&U::argument_type));
  template <typename U> static no_type  test(...);
 public:
  static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
};

template<class F>
auto compose(F f) {
    return [f](auto x) { return f(x); };
}

template <class F, class G>
auto compose(F f, G g){
    return [f,g](auto x){
      std::cout << has_argument_type<F>::value << std::endl;
      if (has_argument_type<F>::value) {
          static constexpr bool value = std::is_same<typename G::result_type, typename F::argument_type>::value;
          static_assert((value == true), "Incompatible types in functions!");
      } else {
          //std::cout << typeid(F1).name() << std::endl;
          //std::function<std::remove_pointer<F>::type> f(f);
          //static constexpr bool value = std::is_same<typename G::result_type, typename decltype(f)::argument_type>::value;
          //static_assert((value == true), "Incompatible types in functions!");
      }
      return f(g(x));
    };
}

template <class F, typename... Fs>
auto compose(F f, Fs&&... fs) {
    return compose(f, compose(fs...));
}

#endif //INC_4_SRC_COMPOSE3_H_
