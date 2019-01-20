#include <logger/Logger.hpp>

using namespace logger;

struct LoggerStackNode {
    Logger logger;
    LoggerStackNode * parent = nullptr;

    operator Logger&() {
        return this->logger;
    }
};

static LoggerStackNode * current_node = nullptr;

namespace logger {
    Logger &get() {
        if (current_node == nullptr) {
            current_node = new LoggerStackNode{Logger()};
        }

        return *current_node;
    }

    Logger &push() {


        current_node = new LoggerStackNode{
            Logger(
                    current_node != nullptr
                    ? &(current_node->logger)
                    : nullptr
            ),
            current_node
        };

        return *current_node;
    }

    void pop() {
        auto * to_delete = current_node;

        if(current_node != nullptr) {
            current_node = current_node->parent;
        }

        delete to_delete;
    }

    Logger &log(FlagType flags, std::string const & message) {
        get().send(flags, message);

        return get();
    }
}
