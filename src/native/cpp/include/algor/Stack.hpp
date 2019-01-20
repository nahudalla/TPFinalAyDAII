#ifndef TPFINALAYDAI_ALGOR_STACK_HPP
#define TPFINALAYDAI_ALGOR_STACK_HPP

#include <algor/List.hpp>

#ifndef __EMSCRIPTEN__
#include <iostream>
#endif

namespace algor {
    template <typename T>
    class Stack {
        List<T> list;

    public:
        void push(T value) {
            this->list.add_front(std::move(value));
        }
        bool isEmpty() const {
            return this->list.isEmpty();
        }
        bool hasNextToTop() const {
            auto it = this->list.begin();
            auto end = this->list.end();

            return it != end && ++it != end;
        }
        const T & top() const {
            return *(this->list.begin());
        }
        const T & nextToTop() const {
            return *(++(this->list.begin()));
        }
        T pop() {
            return this->list.remove(this->list.begin());
        }

        bool operator==(const Stack & rhs) {
            if(this == &rhs) return true;

            return this->list == rhs.list;
        }

        bool operator!=(const Stack & rhs) {
            return !(*this == rhs);
        }

#ifndef __EMSCRIPTEN__
        friend std::ostream &operator<<(std::ostream & out, Stack const& stack) {
            Stack reversed;

            auto it = stack.list.begin();
            auto end = stack.list.end();
            for(; it!=end; ++it) {
                reversed.push(*it);
            }

            return out << reversed.list;
        }
#endif
    };
}

#endif //TPFINALAYDAI_ALGOR_STACK_HPP
