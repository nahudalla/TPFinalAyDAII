#include <third_party/rang.hpp>

#include <iostream>

#include <cli/logger_setup.hpp>

#include <logger/Logger.hpp>

using logger::Logger;

bool critical_message_processor(Logger::Message const&);
bool error_message_processor(Logger::Message const&);
bool warning_message_processor(Logger::Message const&);
bool info_message_processor(Logger::Message const&);

namespace cli {
    void setup_logger() {
        using namespace logger::flags;

        auto & logger = logger::get();

        logger.addListener(CRITICAL, critical_message_processor);
        logger.addListener(INFO_ALL, info_message_processor);
        logger.addListener(WARNING, warning_message_processor);
        logger.addListener(ERROR, error_message_processor);
    }
}

bool critical_message_processor(Logger::Message const& msg) {
    std::cerr << rang::style::bold << rang::fg::red << "  ERROR CRITICO: " << rang::style::reset
              << rang::fg::red << msg.getMessage() << rang::fg::reset << std::endl;

    std::exit(7777);
}

bool error_message_processor(Logger::Message const& msg) {
    std::cerr << rang::style::bold << rang::fg::red << "  ERROR: " << rang::style::reset
              << rang::fg::red << msg.getMessage() << rang::fg::reset << std::endl;

    return false;
}

bool warning_message_processor(Logger::Message const& msg) {
    std::cerr << rang::style::bold << rang::fg::yellow << "  ATENCION: " << rang::style::reset
              << rang::fg::reset << msg.getMessage() << std::endl;

    return false;
}

bool info_message_processor(Logger::Message const & msg) {
    using namespace logger::flags;

    if     ((msg.getFlags() & INFO_1) != 0) std::cout << rang::fg::green;
    else if((msg.getFlags() & INFO_2) != 0) std::cout << rang::fg::cyan;
    else if((msg.getFlags() & INFO_3) != 0) std::cout << rang::fg::blue;
    else if((msg.getFlags() & INFO_4) != 0) std::cout << rang::fg::magenta;
    else if((msg.getFlags() & INFO_5) != 0) std::cout << rang::fg::gray;

    std::cout << rang::style::dim << rang::style::bold << "  INFORMACION: " << rang::fg::reset << rang::style::reset
              << rang::style::dim << msg.getMessage() << rang::style::reset << std::endl;

    return false;
}
