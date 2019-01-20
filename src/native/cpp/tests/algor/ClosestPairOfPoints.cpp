#include "../Test.hpp"

#include <algor/ClosestPairOfPoints.hpp>

#include "TestsLoaderCommon.hpp"

using namespace algor;

bool closest_pair_of_points_instantiation(); ADD_TEST(CLOSEST_PAIR_OF_POINTS_INSTANTIATION, closest_pair_of_points_instantiation);
bool closest_pair_of_points_algorithm(); ADD_TEST(CLOSEST_PAIR_OF_POINTS_ALGORITHM, closest_pair_of_points_algorithm);

ADD_TEST(CLOSEST_PAIR_OF_POINTS_CLEANUP, cleanupTestsLoader<ClosestPairOfPoints>);

bool closest_pair_of_points_instantiation() {
    const auto & lists = getTestsLoader<ClosestPairOfPoints>().getTestCases();

    if(lists.isEmpty()) return false;

    for (int i = 0; i < 20; ++i) {
        auto it = lists.begin();
        auto end = lists.end();

        for(; it != end; ++it) {
            ClosestPairOfPoints asi(*it);
        }
    }

    return true;
}

bool closest_pair_of_points_algorithm() {
    auto & tests = getTestsLoader<ClosestPairOfPoints>().getTestCases();
    auto & solutions = getTestsLoader<ClosestPairOfPoints>().getSolutions();

    if(tests.isEmpty() || solutions.isEmpty()) {
        return false;
    }

    for (int i = 0; i < 20; ++i) {
        auto it_tests = tests.begin();
        auto it_tests_end = tests.end();
        auto it_solutions = solutions.begin();
        auto it_solutions_end = solutions.end();

        for(; it_tests != it_tests_end && it_solutions != it_solutions_end; ++it_tests, ++it_solutions) {
            ClosestPairOfPoints asi(*it_tests);

            auto res = asi.run();

            if(!res || *res != *it_solutions) {
                return false;
            }
        }

        if(it_tests != it_tests_end || it_solutions != it_solutions_end) {
            return false;
        }
    }

    return true;
}
