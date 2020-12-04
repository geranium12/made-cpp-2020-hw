#ifndef INC_4_SRC_COMPOSE_H_
#define INC_4_SRC_COMPOSE_H_

#include <type_traits>
#include <functional>

template<typename R, typename ...Args>
std::function<R(Args...)> compose(const std::function<R(Args...)>& f) {
    return f;
}

template<typename R1, typename Arg1_R2, typename ...Arg2>
std::function<R1(Arg2...)> compose(
    const std::function<R1(Arg1_R2)>& f,
    const std::function<Arg1_R2(Arg2...)>& g) {
    std::function<R1(Arg2...)> result = [f, g](auto ...arg) {
        return f(g(arg...));
    };

    return result;
}

template<typename R1, typename Arg1_R2, typename Arg2, typename ...Args>
std::function<R1(Arg2)> compose(
    const std::function<R1(Arg1_R2)>& f,
    const std::function<Arg1_R2(Arg2)>& g,
    Args ...args) {
    std::function<R1(Arg2)> result = [f, g](auto... arguments) {
        return f(g(arguments...));
    };

    return compose(result, args...);
}

#endif //INC_4_SRC_COMPOSE_H_

