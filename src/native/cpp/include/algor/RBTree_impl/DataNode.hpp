#ifndef TPFINALAYDAI_RBTREE_IMPL_DATANODE_HPP
#define TPFINALAYDAI_RBTREE_IMPL_DATANODE_HPP

#include <algorithm>

#include <algor/RBTree_impl/Color.hpp>
#include <algor/RBTree_impl/Node.hpp>

namespace algor::__detail_RBTree {
    template <typename T>
    class DataNode : public Node<T> {
        T data;

    public:
        explicit DataNode(T data)
            : data(std::move(data))
        {}

        DataNode(T data, Node<T> * parent, Node<T> * left, Node<T> * right, Color color)
                : Node<T>(parent, left, right, color), data(std::move(data))
        {}

        DataNode(DataNode const&) = default;
        DataNode(DataNode &&) noexcept = default;
        DataNode &operator=(DataNode const&) = default;
        DataNode &operator=(DataNode &&) noexcept = default;
        ~DataNode() = default;

        Node<T> *clone() const override {
            return new DataNode(*this);
        }

        const T *getData() const override {
            return &(this->data);
        }

        T *getData() override {
            return &(this->data);
        }
    };
}

#endif //TPFINALAYDAI_RBTREE_IMPL_DATANODE_HPP
