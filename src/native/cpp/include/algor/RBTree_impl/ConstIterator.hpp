#ifndef TPFINALAYDAI_RBTREE_IMPL_CONSTITERATOR_HPP
#define TPFINALAYDAI_RBTREE_IMPL_CONSTITERATOR_HPP

#include <algor/RBTree_impl/Node.hpp>

namespace algor {
    template <typename T>
    class RBTree;
}

namespace algor::__detail_RBTree {
    template <typename T, typename Node>
    class IteratorBase {
    protected:
        Node * node = Node::Nil::get();

        explicit IteratorBase(Node *node) : node(node) {}

    public:
        IteratorBase() = default;
        IteratorBase(IteratorBase const&) = default;
        IteratorBase(IteratorBase &&) noexcept = default;
        IteratorBase &operator=(IteratorBase const&) = default;
        IteratorBase &operator=(IteratorBase &&) noexcept = default;
        ~IteratorBase() = default;

        bool operator==(const IteratorBase &rhs) const {
            return this->node == rhs.node;
        }

        bool operator!=(const IteratorBase &rhs) const {
            return !(rhs == *this);
        }
    };

    template <typename T, typename Node = const Node<T>, bool reverse = false>
    class ConstIterator : public IteratorBase<T, Node> {
    protected:
        explicit ConstIterator(Node * node) : IteratorBase<T, Node>(node)
        {}

        friend class RBTree<T>;
    private:
        void next_impl() {
            this->node = this->node->next();
        }

        void previous_impl() {
            this->node = this->node->previous();
        }

    public:
        ConstIterator() = default;
        ConstIterator(ConstIterator const&) = default;
        ConstIterator(ConstIterator &&) noexcept = default;
        ConstIterator &operator=(ConstIterator const&) = default;
        ConstIterator &operator=(ConstIterator &&) noexcept = default;
        ~ConstIterator() = default;

        explicit ConstIterator(IteratorBase<T, Node> const& other)
            : IteratorBase<T, Node>(other)
        {}

        bool isDereferentiable() const {
            return this->node->getData() != nullptr;
        }

        operator bool() const {
            return this->isDereferentiable();
        }

        T const&operator*() const {
            return *(this->node->getData());
        }

        const T *operator->() const {
            return this->node->getData();
        }

        ConstIterator &next() {
            if constexpr (!reverse) {
                this->next_impl();
            } else {
                this->previous_impl();
            }

            return *this;
        }

        ConstIterator &operator++() {
            return this->next();
        }

        const ConstIterator operator++(int) {
            auto tmp = *this;
            this->next();
            return std::move(tmp);
        }

        ConstIterator &previous() {
            if constexpr (!reverse) {
                this->previous_impl();
            } else {
                this->next_impl();
            }

            return *this;
        }

        ConstIterator &operator--() {
            return this->previous();
        }

        const ConstIterator operator--(int) {
            auto tmp = *this;
            this->previous();
            return std::move(tmp);
        }
    };

    template <typename T, typename Node = const Node<T>>
    using ReverseConstIterator = ConstIterator<T, Node, true>;
}

#endif //TPFINALAYDAI_RBTREE_IMPL_CONSTITERATOR_HPP
