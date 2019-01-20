#ifndef TPFINALAYDAI_LOGGER_HPP
#define TPFINALAYDAI_LOGGER_HPP

#include <utility>
#include <string>
#include <functional>
#include <optional>

#include <algor/List.hpp>

#include <logger/Logger_impl/LogTypes.hpp>
#include <logger/Logger_impl/Message.hpp>

namespace algor {
    class GeometricObject;
}

namespace logger {
    class Logger;

    namespace flags = __detail_Logger::flags;
    using FlagType = __detail_Logger::FlagType;

    Logger &get();
    Logger &push();
    void pop();
    Logger &log(FlagType, std::string const&);

    class Logger {
        using OwnerMessage = __detail_Logger::OwnerMessage;

    public:
        using Message = __detail_Logger::Message;
        using Listener = std::function<bool(Message const&)>;

    private:
        Logger * parent;

        bool propagate_ = true;

        algor::List<std::pair<FlagType, Listener>> listeners;

        std::optional<algor::List<Message *>> history;

        explicit Logger(Logger * parent = nullptr)
            : parent(parent)
        {}

        Logger(Logger &&) noexcept = default;
        Logger &operator=(Logger &&) noexcept = default;

        friend Logger &get();
        friend Logger &push();
    public:
        Logger(Logger const&) = delete;
        Logger &operator=(Logger const&) = delete;
        ~Logger() {
            if(this->history.has_value()) {
                while(!this->history->isEmpty()) {
                    delete this->history->remove(
                            this->history->begin()
                    );
                }
                this->history = std::nullopt;
            }
        }

        Logger &propagate(bool value = true) {
            this->propagate_ = value;
            return *this;
        }
        bool propagationEnabled() const {
            return this->propagate_;
        }

        Logger &addListener(FlagType flags, Listener listener) {
            this->listeners.add({flags, std::move(listener)});
            return *this;
        }

        bool send(FlagType flags, std::string const& message) const {
            Message m(flags, &message);

            if(this->history.has_value()) {
                const_cast<Logger*>(this)->history->add(new OwnerMessage(m));
            }

            auto it = this->listeners.begin();
            auto end = this->listeners.end();
            for(; it!=end; ++it) {
                if((it->first & flags) != 0x0) {
                    if(it->second(m)) return true;
                }
            }

            if(this->propagate_ && this->parent != nullptr) {
                return this->parent->send(flags, message);
            }

            return false;
        }

        Logger &enableHistory(bool value = true) {
            if(value) this->history = algor::List<Message *>();
            else this->history = std::nullopt;

            return *this;
        }
        bool historyEnabled() const {
            return this->history.has_value();
        }

        const auto &getHistory() const {
            return this->history;
        }

        auto extractHistory() {
            auto his = std::move(*(this->history));
            this->history = std::nullopt;
            return std::move(his);
        }
    };

    class LoggerContextHolder {
    public:
        LoggerContextHolder() {push();}

        LoggerContextHolder(LoggerContextHolder const&) = delete;
        LoggerContextHolder(LoggerContextHolder &&) noexcept = delete;
        LoggerContextHolder &operator=(LoggerContextHolder const&) = delete;
        LoggerContextHolder &operator=(LoggerContextHolder &&) noexcept = delete;

        ~LoggerContextHolder() {pop();}

        Logger &operator*() {return get();}
        Logger &operator->() {return get();}
    };
}

#endif //TPFINALAYDAI_LOGGER_HPP
