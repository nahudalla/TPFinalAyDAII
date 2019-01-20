#include "../Test.hpp"

#include <algor/List.hpp>
#include <algor/Segment.hpp>
#include <algor/AnySegmentsIntersect.hpp>

#include "TestsLoaderCommon.hpp"

using namespace algor;

bool any_segments_intersect_instantiation(); ADD_TEST(ANY_SEGMENTS_INTERSECT_INSTANTIATION, any_segments_intersect_instantiation);
bool any_segments_intersect_algorithm(); ADD_TEST(ANY_SEGMENTS_INTERSECT_ALGORITHM, any_segments_intersect_algorithm);

ADD_TEST(ANY_SEGMENTS_INTERSECT_CLEANUP, cleanupTestsLoader<AnySegmentsIntersect>);

bool any_segments_intersect_instantiation() {
    const auto & lists = getTestsLoader<AnySegmentsIntersect>().getTestCases();

    if(lists.isEmpty()) return false;

    for (int i = 0; i < 20; ++i) {
        auto it = lists.begin();
        auto end = lists.end();

        for(; it != end; ++it) {
            AnySegmentsIntersect asi(*it);
        }
    }

    return true;
}

bool any_segments_intersect_algorithm() {
    auto & tests = getTestsLoader<AnySegmentsIntersect>().getTestCases();
    auto & solutions = getTestsLoader<AnySegmentsIntersect>().getSolutions();

    if(tests.isEmpty() || solutions.isEmpty()) {
        return false;
    }

    for (int i = 0; i < 20; ++i) {
        auto it_tests = tests.begin();
        auto it_tests_end = tests.end();
        auto it_solutions = solutions.begin();
        auto it_solutions_end = solutions.end();

        for(; it_tests != it_tests_end && it_solutions != it_solutions_end; ++it_tests, ++it_solutions) {
            AnySegmentsIntersect asi(*it_tests);

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
