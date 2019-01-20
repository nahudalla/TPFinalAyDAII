#ifndef TPFINALAYDAI_RBTREE_IMPL_NIL_HPP
#define TPFINALAYDAI_RBTREE_IMPL_NIL_HPP

#include <algor/RBTree_impl/Color.hpp>
#include <algor/RBTree_impl/Node.hpp>

namespace algor::__detail_RBTree {
    template <typename T>
    class Nil : public Node<T> {
        Nil() : Node<T>(this, this, this) {
            this->setParent(this);
            this->setLeft(this);
            this->setRight(this);
            this->setColor(BLACK);
        }

    public:
        const T *getData() const override { return nullptr; }
        T *getData() override { return nullptr; }

        Node<T> * clone() const override {
            return this->to_non_const();
        }

        static Node<T> * get() {
            static Node<T> * nil = new Nil();

            return nil;
        }
    };
}

#endif //TPFINALAYDAI_RBTREE_IMPL_NIL_HPP
