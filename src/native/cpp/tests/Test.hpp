#ifndef TESTS_TEST_HPP
#define TESTS_TEST_HPP

#include <functional>
#include <list>
#include <string>

namespace Tests {
    struct Test {
        std::string name;
        std::function<bool()> test;

        Test(std::string name, std::function<bool()> test);
        Test(Test const&) = default;
        Test(Test &&) noexcept = default;
        Test &operator=(Test const&) = default;
        Test &operator=(Test &&) noexcept = default;
        ~Test() = default;

        static std::list<Test> tests;
    };
}

#define ADD_TEST(name, def) \
struct Test_ ## name {\
    static char _init;\
    static char init() noexcept { \
        try{Tests::Test::tests.push_back(::Tests::Test(#name, std::function<bool()>(def)));}catch(...){} \
        return ' '; \
    } \
}; \
char Test_ ## name::_init = Test_ ## name::init()

#endif //TESTS_TEST_HPP