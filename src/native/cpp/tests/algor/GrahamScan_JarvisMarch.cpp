#include "../Test.hpp"

#include <algor/List.hpp>
#include <algor/Stack.hpp>
#include <algor/Point.hpp>
#include <algor/GrahamScan.hpp>
#include <algor/JarvisMarch.hpp>

#include <sstream>

#include "TestsLoaderCommon.hpp"

using namespace algor;

bool graham_scan_instantiate(); ADD_TEST(GRAHAM_SCAN_INSTANTIATE, graham_scan_instantiate);
bool graham_scan_algorithm(); ADD_TEST(GRAHAM_SCAN_ALGORITHM, graham_scan_algorithm);

ADD_TEST(GRAHAM_SCAN_CLEANUP, cleanupTestsLoader<GrahamScan>);

bool jarvis_march_instantiate(); ADD_TEST(JARVIS_MARCH_INSTANTIATE, jarvis_march_instantiate);
bool jarvis_march_algorithm(); ADD_TEST(JARVIS_MARCH_ALGORITHM, jarvis_march_algorithm);

ADD_TEST(JARVIS_MARCH_CLEANUP, cleanupTestsLoader<JarvisMarch>);

template <typename T>
bool instantiation_test() {
    auto & tests = getTestsLoader<T>().getTestCases();

    for (int i = 0; i < 20; ++i) {
        auto it = tests.begin();
        auto end = tests.end();

        if(it == end) return false;

        for(; it != end; ++it) {
            T gs(*it);

            if (!gs.canRun() || gs.hasResult()) return false;
        }
    }

    return true;
}

template <typename T>
bool algorithm_test() {
    auto & tests = getTestsLoader<T>().getTestCases();
    auto & solutions = getTestsLoader<T>().getSolutions();

    for (int i = 0; i < 20; ++i) {
        auto it_tests = tests.begin();
        auto it_tests_end = tests.end();
        auto it_solutions = solutions.begin();
        auto it_solutions_end = solutions.end();

        if(it_tests == it_tests_end || it_solutions == it_solutions_end) {
            return false;
        }

        for(; it_tests != it_tests_end && it_solutions != it_solutions_end; ++it_tests, ++it_solutions) {
            T gs(*it_tests);

            auto res = gs.run();

            if(!res.has_value()) return false;

            if(*res != *it_solutions) return false;
        }

        if(it_tests != it_tests_end || it_solutions != it_solutions_end) {
            return false;
        }
    }

    return true;
}

bool graham_scan_instantiate() {
    return instantiation_test<GrahamScan>();
}

bool graham_scan_algorithm() {
    return algorithm_test<GrahamScan>();
}

bool jarvis_march_instantiate() {
    return instantiation_test<JarvisMarch>();
}

bool jarvis_march_algorithm() {
    return algorithm_test<JarvisMarch>();
}
