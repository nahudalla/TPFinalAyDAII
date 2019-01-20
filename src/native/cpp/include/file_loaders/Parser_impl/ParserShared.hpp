#ifndef TPFINALAYDAI_TEST_CASES_PARSERSHARED_HPP
#define TPFINALAYDAI_TEST_CASES_PARSERSHARED_HPP

#include <algor/Point.hpp>
#include <istream>
#include <optional>

namespace file_loaders::__detail_Parser {
    std::optional<algor::Point> parse_point(std::istream & input);
}

#endif //TPFINALAYDAI_TEST_CASES_PARSERSHARED_HPP
