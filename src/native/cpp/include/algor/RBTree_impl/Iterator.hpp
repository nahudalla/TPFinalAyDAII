#ifndef TPFINALAYDAI_RBTREE_IMPL_ITERATOR_HPP
#define TPFINALAYDAI_RBTREE_IMPL_ITERATOR_HPP

#include <algor/RBTree_impl/Node.hpp>
#include <algor/RBTree_impl/ConstIterator.hpp>

namespace algor {
    template <typename T>
    class RBTree;
}

namespace algor::__detail_RBTree {
    template <typename T, bool reverse = false>
    class Iterator : public ConstIterator<T, Node<T>, reverse> {
        typedef ConstIterator<T, Node<T>, reverse> Base;

        explicit Iterator(Node<T> * node) : ConstIterator<T, Node<T>>(node)
        {}

        friend class RBTree<T>;

    public:
        Iterator() = default;
        Iterator(Iterator const&) = default;
        Iterator(Iterator &&) noexcept = default;
        Iterator &operator=(Iterator const&) = default;
        Iterator &operator=(Iterator &&) noexcept = default;
        ~Iterator() = default;

        explicit Iterator(IteratorBase<T, Node<T>> const& other)
                : ConstIterator<T, Node<T>, reverse>(other)
        {}

        T &operator*() {
            return *(this->node->getData());
        }

        T *operator->()  {
            return this->node->getData();
        }

        Iterator &next() {
            Base::next();
            return *this;
        }

        Iterator &operator++() {
            return this->next();
        }

        const Iterator operator++(int) {
            auto tmp = *this;
            this->next();
            return std::move(tmp);
        }

        Iterator &previous() {
            Base::previous();
            return *this;
        }

        Iterator &operator--() {
            return this->previous();
        }

        const Iterator operator--(int) {
            auto tmp = *this;
            this->previous();
            return std::move(tmp);
        }
    };

    template <typename T>
    using ReverseIterator = Iterator<T, true>;
}

#endif //TPFINALAYDAI_RBTREE_IMPL_ITERATOR_HPP
