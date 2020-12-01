#ifndef INC_4_SRC_COMPOSE2_H_
#define INC_4_SRC_COMPOSE2_H_

template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
{
  enum { arity = sizeof...(Args) };

  typedef ReturnType result_type;

  template <size_t i>
  struct arg
  {
    typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
  };
};

template<typename F>
auto compose(F f) {
    return [f](auto x) { return f(x); };
}

template<class F, typename... G>
auto compose(F f, G... g) {
    return compose(f, compose(g...));
}

template <typename T>
class has_argument_type {
  typedef char yes_type;
  typedef long no_type;
  template <typename U> static yes_type test(decltype(&U::argument_type));
  template <typename U> static no_type  test(...);
 public:
  static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
};

template<typename F1, typename F2>
auto compose(F1 f1, F2 f2) {
    return [f1, f2](auto x) {
        //std::cout << typeid(F1).name() << std::endl;
        //std::cout << typeid(std::function<std::remove_pointer<F2>::type>).name() << std::endl;
        if (has_argument_type<F1>::value) {
            //typedef function_traits<decltype(f2)> traits;
            //std::cout << typeid(traits::arg<1>::type).name() << std::endl;
            std::function<std::remove_pointer<F2>::type> func(f1);
            static constexpr bool value = std::is_same<typename decltype(func)::result_type, typename F1::argument_type>::value;
            static_assert((value == true), "Incompatible types in functions!");
        } else {
            //std::cout << typeid(F1).name() << std::endl;
            //std::function<std::remove_pointer<F1>::type> f(f1);
            //static constexpr bool value = std::is_same<typename F2::result_type, typename decltype(f)::argument_type>::value;
            //static_assert((value == true), "Incompatible types in functions!");
        }
        return f1(f2(x));
    };
}

#endif //INC_4_SRC_COMPOSE2_H_
