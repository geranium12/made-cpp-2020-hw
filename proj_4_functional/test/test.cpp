#include <iostream>
#include <functional>

#include "src/function.h"
#include "src/compose.h"

void FailWithMsg(const std::string& msg, int line) {
    std::cerr << "Test failed!\n";
    std::cerr << "[Line " << line << "] "  << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

#define ASSERT_TRUE(cond) \
    if (!(cond)) {FailWithMsg("Assertion failed: " #cond, __LINE__);};

#define ASSERT_TRUE_MSG(cond, msg) \
    if (!(cond)) {FailWithMsg(msg, __LINE__);};

#define ASSERT_EQUAL_MSG(a, b, msg) \
    ASSERT_TRUE_MSG(a == b, msg)


int sum2(const int* x, int y) {
    return (*x) + y;
}

typedef function<int (const int* , int)> int_function_with_two_args_t;

struct Foo {
  int num_;
  explicit Foo(int num) : num_(num) {}
  int add(int i) const {
      return num_ + i;
  }
};

int get_new_number(int i) {
    return i + 1;
}

struct GetNewNumber {
  int operator()(int i) const {
      return i + 10;
  }
};

int plus_function(int a, int b) { return a+b; }
int minus_function(int a, int b) { return a-b; }


int main()
{
    // FUNCTION TESTS

    {
        function<int(int)> f_get = get_new_number;
        ASSERT_EQUAL_MSG(-8, f_get(-9), "Store a free function");
    }

    {
        function<int()> f_get_42 = []() { return get_new_number(42); };
        ASSERT_EQUAL_MSG(43, f_get_42(), "Store a lambda");
    }

    {
        int x = 10;
        int_function_with_two_args_t f_sub = [](const int* a, int b) -> int { return *a - b; };
        ASSERT_EQUAL_MSG(4, f_sub(&x, 6), "Store a lambda");
    }

    {
        function<int()> f_get_31337 = std::bind(get_new_number, 31337);
        ASSERT_EQUAL_MSG(31338, f_get_31337(), "Store the result of a call to std::bind");
    }

    {
        int_function_with_two_args_t f2(sum2);
        int x = 10;
        ASSERT_EQUAL_MSG(30, f2(&x, 20), "Calling a function with two args");
    }

    {
        std::function<int(const Foo&, int)> f_add = &Foo::add;
        const Foo foo(314159);
        ASSERT_EQUAL_MSG(314160, f_add(foo, 1), "Store a call to a member function. "
                                                "Calling with struct, int");
    }

    {
        std::function<int(Foo const&)> f_num = &Foo::num_;
        const Foo foo(314159);
        ASSERT_EQUAL_MSG(314159, f_num(foo), "Store a call to a data member accessor");
    }

    {
        using std::placeholders::_1;
        const Foo foo(314159);
        function<int(int)> f_add_2 = std::bind(&Foo::add, foo, _1);
        ASSERT_EQUAL_MSG(314161, f_add_2(2), "Store a call to a member function and object");
    }

    {
        using std::placeholders::_1;
        const Foo foo(314159);
        function<int(int)> f_add_3 = std::bind(&Foo::add, &foo, _1);
        ASSERT_EQUAL_MSG(314162, f_add_3(3), "Store a call to a member function and object ptr");
    }

    {
        function<int(int)> f_obj = GetNewNumber();
        ASSERT_EQUAL_MSG(28, f_obj(18), "Store a call to a function object");
    }

    {
        int x = 10;
        int_function_with_two_args_t f1(sum2);
        int_function_with_two_args_t f2(f1);
        int_function_with_two_args_t f3(std::move(f2));
        ASSERT_EQUAL_MSG(18, f3(&x, 8), "Check move constructor");
        ASSERT_TRUE_MSG(f1.operator bool(), "Check copy constructor");
        ASSERT_TRUE_MSG(f3.operator bool(), "Check move constructor");
        ASSERT_TRUE_MSG(!f2.operator bool(), "Check move constructor");
    }

    {
        int x = 10;
        int_function_with_two_args_t f1(sum2);
        int_function_with_two_args_t f2 = f1;
        int_function_with_two_args_t f3 = f1;
        f3.operator=(nullptr);
        ASSERT_EQUAL_MSG(17, f2(&x, 7), "Check operator=");
        ASSERT_EQUAL_MSG(17, f1(&x, 7), "Check operator=");
        ASSERT_TRUE_MSG(!f3, "Check operator=");
    }

    {
        int x = 10;
        int_function_with_two_args_t f_add = [](const int* a, int b) -> int { return *a + b; };
        int_function_with_two_args_t f_sub = [](const int* a, int b) -> int { return *a - b; };
        ASSERT_EQUAL_MSG(14, f_add(&x, 4), "Store a lambda");
        ASSERT_EQUAL_MSG(3, f_sub(&x, 7), "Store a lambda");
        std::swap(f_add, f_sub);
        ASSERT_EQUAL_MSG(14, f_sub(&x, 4), "Check swap");
        ASSERT_EQUAL_MSG(3, f_add(&x, 7), "Check swap");
        f_add.swap(f_sub);
        ASSERT_EQUAL_MSG(14, f_add(&x, 4), "Check swap");
        ASSERT_EQUAL_MSG(3, f_sub(&x, 7), "Check swap");
    }

    {
        function<int(int,int)> plus1  = plus_function;
        function<int(int,int)> plus2  = std::plus<int>();
        function<int(int,int)> minus1 = minus_function;
        function<int(int,int)> minus2 = std::minus<int>();

        ASSERT_TRUE_MSG((plus1.target_type()==typeid(int(*)(int,int))), "Check target_type()");
        ASSERT_TRUE_MSG(!(plus2.target_type()==typeid(int(*)(int,int))), "Check target_type()");
        ASSERT_TRUE_MSG((minus1.target_type()==typeid(int(*)(int,int))), "Check target_type()");
        ASSERT_TRUE_MSG(!(minus2.target_type()==typeid(int(*)(int,int))), "Check target_type()");

        ASSERT_TRUE_MSG(!(plus1.target_type()== plus2.target_type()), "Check target_type()");
        ASSERT_TRUE_MSG(!(minus1.target_type()==minus2.target_type()), "Check target_type()");
        ASSERT_TRUE_MSG((plus1.target_type()==minus1.target_type()), "Check target_type()");
        ASSERT_TRUE_MSG(!(plus2.target_type()==minus2.target_type()), "Check target_type()");
    }

    // COMPOSE TESTS

    std::function<bool(int)> a = [] (int x) -> bool {
      return x > 10;
    };

    std::function<int(float)> b = [] (float y) -> int {
      return int(y * y);
    };

    std::function<float(bool)> c = [] (bool z) -> float {
      return z ? 3.1f : 3.34f;
    };

    std::function<float(float, bool, float)> d = [] (float a, bool b, float c) -> float {
        return b ? a + c : a - c;
    };

    {
        auto e = compose(a, b, c, a, b, c);
        ASSERT_EQUAL_MSG(1, e(true), "Check compose()");
    }

    {
        auto e = compose(a, b, c);
        ASSERT_TRUE_MSG(!e(true), "Check compose()");
    }

    {
        auto e = compose(b, d);
        ASSERT_EQUAL_MSG(25, e(1.2, true, 3.8), "Check compose()");
    }

    return 0;
}