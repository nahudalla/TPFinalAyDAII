#ifndef TPFINALAYDAI_LSIT_IMPL_ITERATOR_HPP
#define TPFINALAYDAI_LSIT_IMPL_ITERATOR_HPP

#include <algorithm>

#include <algor/List_impl/Node.hpp>
#include <algor/List_impl/ConstIterator.hpp>

namespace algor {
    template <typename> class List;
}

namespace algor::__detail__List {
    template <typename T>
    class Iterator : public ConstIterator<T, Node<T>> {
        typedef Node<T> Node;
        typedef ConstIterator<T, Node> Base;

        Iterator(Node *prev, Node *curr) : Base(prev, curr) {}

        template <typename TNode, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<TNode>, Node>>>
        explicit Iterator(ConstIterator<T, TNode> const& other) : Base(other)
        {}
        template <typename TNode, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<TNode>, Node>>>
        explicit Iterator(ConstIterator<T, TNode> && other) : Base(std::move(other))
        {}

        friend class List<T>;

    public:
        Iterator() = default;

        Iterator(Iterator const &) = default;
        Iterator(Iterator &&) noexcept = default;
        Iterator &operator=(Iterator const &) = default;
        Iterator &operator=(Iterator &&) noexcept = default;
        ~Iterator() = default;

        Iterator &next() {
            Base::next();
            return *this;
        }

        Iterator &operator++() {
            this->next();
            return *this;
        }

        const Iterator operator++(int) {
            auto tmp = *this;
            this->next();
            return std::move(tmp);
        }

        T &operator*() {
            return this->curr->elem;
        }

        T *operator->() {
            return &(this->curr->elem);
        }
    };
}

#endif //TPFINALAYDAI_LSIT_IMPL_ITERATOR_HPP
