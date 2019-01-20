#ifndef TPFINALAYDAI_LOGTYPES_HPP
#define TPFINALAYDAI_LOGTYPES_HPP

#include <bitset>
#include <stdexcept>

#undef ERROR

namespace logger::__detail_Logger {
    constexpr const std::size_t MAX_FLAGS = 10;

    typedef std::bitset<MAX_FLAGS> FlagType;

    namespace flags {
        static const FlagType CRITICAL         = 1 << 0;
        static const FlagType ERROR            = 1 << 1;
        static const FlagType INFO_1           = 1 << 2;
        static const FlagType INFO_2           = 1 << 3;
        static const FlagType INFO_3           = 1 << 4;
        static const FlagType INFO_4           = 1 << 5;
        static const FlagType INFO_5           = 1 << 6;
        static const FlagType INFO             = INFO_1;
        static const FlagType INFO_ALL         = INFO_1 | INFO_2 | INFO_3 | INFO_4 | INFO_5;
        static const FlagType WARNING          = 1 << 7;
    }
}

#endif //TPFINALAYDAI_LOGTYPES_HPP
