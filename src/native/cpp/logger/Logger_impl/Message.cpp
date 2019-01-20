#include <logger/Logger_impl/Message.hpp>

#include <algor/GeometricObject.hpp>

namespace logger::__detail_Logger {
    void Message::free_memory() {
        delete this->message;
        this->message = nullptr;

        if(this->attached_objects.has_value()) {
            while(!this->attached_objects->isEmpty()) {
                delete this->attached_objects->remove(
                        this->attached_objects->begin()
                );
            }
            this->attached_objects = std::nullopt;
        }
    }

    OwnerMessage::OwnerMessage(Message const& message) {
        this->setMessage(new std::string(message.getMessage()));
        this->setFlags(message.getFlags());

        if(message.hasAttachments()) {
            algor::List<algor::GeometricObject *> objects;

            auto it = message.getAttachments().begin();
            auto end = message.getAttachments().end();
            for(; it!=end; ++it) {
                objects.add((*it)->clone());
            }

            this->setAttachedObjects(std::move(objects));
        }
    }
}
