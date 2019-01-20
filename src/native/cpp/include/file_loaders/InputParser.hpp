#ifndef TPFINALAYDAI_TEST_CASES_TEST_CASE_PARSER_HPP
#define TPFINALAYDAI_TEST_CASES_TEST_CASE_PARSER_HPP

#include <utility>
#include <istream>
#include <optional>

#include <algor/AnySegmentsIntersect.hpp>
#include <algor/GrahamScan.hpp>
#include <algor/JarvisMarch.hpp>
#include <algor/ClosestPairOfPoints.hpp>
#include <algor/List.hpp>
#include <algor/Point.hpp>
#include <algor/Segment.hpp>

#include <file_loaders/Parser_impl/ParserShared.hpp>

namespace file_loaders {
    template <typename>
    struct InputParser;

    template <>
    struct InputParser<void> {
        typedef void InputType;
    };

    template <>
    struct InputParser<algor::AnySegmentsIntersect> {
        InputParser() = delete;

        typedef std::optional<algor::List<algor::Segment>> InputType;
        typedef InputType ResultType;

        static InputType parse(std::istream &input) {
            InputType res = InputType::value_type();

            std::optional<algor::Point> from = __detail_Parser::parse_point(input),
                                        to = __detail_Parser::parse_point(input);

            while(from.has_value() && to.has_value()) {
                res->add(algor::Segment(*from, *to));

                from = __detail_Parser::parse_point(input);
                to = __detail_Parser::parse_point(input);
            }

            return std::move(res);
        }
    };

    template <>
    struct InputParser<algor::GrahamScan> {
        InputParser() = delete;

        typedef std::optional<algor::List<algor::Point>> InputType;
        typedef InputType ResultType;

        static InputType parse(std::istream &input) {
            InputType res = InputType::value_type();

            std::optional<algor::Point> point = __detail_Parser::parse_point(input);

            while(point.has_value()) {
                res->add(*point);
                point = __detail_Parser::parse_point(input);
            }

            return std::move(res);
        }
    };

    template <>
    struct InputParser<algor::JarvisMarch> {
        InputParser() = delete;

        typedef InputParser<algor::GrahamScan>::InputType InputType;
        typedef InputType ResultType;

        static InputType parse(std::istream &input) {
            return InputParser<algor::GrahamScan>::parse(input);
        }
    };

    template <>
    struct InputParser<algor::ClosestPairOfPoints> {
        InputParser() = delete;

        typedef std::optional<algor::Array<algor::Point>> InputType;
        typedef InputType ResultType;

        static InputType parse(std::istream &input) {
            return algor::Array(*InputParser<algor::GrahamScan>::parse(input));
        }
    };
}

#endif //TPFINALAYDAI_TEST_CASES_TEST_CASE_PARSER_HPP
