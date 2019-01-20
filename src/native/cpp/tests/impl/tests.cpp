#include <iostream>
#include <memory>

#include "../Test.hpp"

int main() {
    std::size_t failed = 0, succeeded = 0;
    for(auto const& test : Tests::Test::tests) {
        std::cout << "RUNNING TEST " << test.name << " ... " << std::flush;
        bool ok = false; bool error = false;
        try { ok = test.test();
        }catch(...){ error = true; }

        if(ok) {
            std::cout << "OK" << std::endl;
            succeeded++;
        } else {
            std::cout << "FAILED" << (error?" (ERROR THROWN)":"") << std::endl;
            failed++;
        }
    }

    std::cout << std::endl;

    std::cout << succeeded << " TEST" << ((succeeded != 1) ? "S" : "") << " SUCCEEDED" << std::endl;
    std::cout << failed    << " TEST" << ((failed    != 1) ? "S" : "") << " FAILED"    << std::endl;

    return (int) failed;
}

Tests::Test::Test(std::string name, std::function<bool()> test)
        : name(std::move(name)), test(std::move(test))
{}
