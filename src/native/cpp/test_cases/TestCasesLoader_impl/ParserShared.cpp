#include <algor/Point.hpp>
#include <istream>
#include <optional>

#include <file_loaders/Parser_impl/ParserShared.hpp>

namespace file_loaders::__detail_Parser {
    std::optional<algor::Point> parse_point(std::istream & input) {
        if(!input) return std::nullopt;

        int x, y;
        input >> x;

        if(!input) return std::nullopt;

        input >> y;

        return algor::Point(x, y);
    }
}
