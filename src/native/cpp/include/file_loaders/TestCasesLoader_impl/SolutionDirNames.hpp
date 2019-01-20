#ifndef TPFINALAYDAI_TEST_CASES_SOLUTION_DIR_NAME_HPP
#define TPFINALAYDAI_TEST_CASES_SOLUTION_DIR_NAME_HPP

#include <algor/AnySegmentsIntersect.hpp>
#include <algor/GrahamScan.hpp>
#include <algor/JarvisMarch.hpp>
#include <algor/ClosestPairOfPoints.hpp>

namespace file_loaders::__detail_TestCasesLoader {
    template <typename = void>
    struct SolutionDirName;

    template <>
    struct SolutionDirName<void> {
        static constexpr const char * DIR_NAME = nullptr;
    };

    template <>
    struct SolutionDirName<algor::AnySegmentsIntersect> {
        static constexpr const char * DIR_NAME = "segments_intersection";
    };

    template <>
    struct SolutionDirName<algor::GrahamScan> {
        static constexpr const char * DIR_NAME = "convex_hull";
    };

    template <>
    struct SolutionDirName<algor::JarvisMarch> {
        static constexpr const char * DIR_NAME = "convex_hull";
    };

    template <>
    struct SolutionDirName<algor::ClosestPairOfPoints> {
        static constexpr const char * DIR_NAME = "closest_points";
    };
}

#endif //TPFINALAYDAI_TEST_CASES_SOLUTION_DIR_NAME_HPP
