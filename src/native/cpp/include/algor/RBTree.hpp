#ifndef TPFINALAYDAI_ALGOR_RBTREE_HPP
#define TPFINALAYDAI_ALGOR_RBTREE_HPP

#include <algorithm>

#include <algor/RBTree_impl/Color.hpp>
#include <algor/RBTree_impl/Node.hpp>
#include <algor/RBTree_impl/DataNode.hpp>
#include <algor/RBTree_impl/Iterator.hpp>
#include <algor/RBTree_impl/ConstIterator.hpp>
#include <algor/Comparator.hpp>

namespace algor {
    template <typename T>
    class RBTree {
        typedef __detail_RBTree::Node<T> Node;
        typedef __detail_RBTree::DataNode<T> DataNode;

        Node * root = Node::Nil::get();
        Comparator<T> comparator;
    public:
        typedef __detail_RBTree::Iterator<T> Iterator;
        typedef __detail_RBTree::ConstIterator<T> ConstIterator;
        typedef __detail_RBTree::ReverseIterator<T> ReverseIterator;
        typedef __detail_RBTree::ReverseConstIterator<T> ReverseConstIterator;

        RBTree() = default;
        explicit RBTree(const Comparator<T> & comparator)
            : comparator(comparator)
        {}
        RBTree(RBTree const& other) {*this = other;}
        RBTree(RBTree && other) noexcept {*this = std::move(other);}
        RBTree &operator=(RBTree const& other) {
            if(this != &other) {
                this->~RBTree();
                this->root = other.root->clone();
                this->comparator = other.comparator;
            }
            return *this;
        }
        RBTree &operator=(RBTree && rhs) noexcept {
            if(this != &rhs) {
                std::swap(this->root, rhs.root);
                std::swap(this->comparator, rhs.comparator);
            }

            return *this;
        }
        ~RBTree() {
            if(!this->root->isNil()) {
                delete this->root;
                this->root = Node::Nil::get();
            }
        }

        auto size() const {
            return this->root->size();
        }

        auto minimum() const {
            return ConstIterator(this->root->minimum());
        }
        auto minimum() {
            return Iterator(this->root->minimum());
        }

        auto maximum() const {
            return ConstIterator(this->root->maximum());
        }
        auto maximum() {
            return Iterator(this->root->maximum());
        }

        auto search(T const& value) const {
            return ConstIterator(this->root->search(value, this->comparator));
        }
        auto search(T const& value) {
            return Iterator(this->root->search(value, this->comparator));
        }

        auto insert(T value) {
            Node * node = new DataNode(value);
            node->insert(this->root, this->comparator);

            return Iterator(node);
        }

        void remove(Iterator const& it) {
            it.node->remove(this->root);
        }
        void remove(T const& value) {
            auto it = this->search(value);

            if(it == this->end()) return;

            this->remove(it);
        }

        auto begin() const { return this->minimum(); }
        auto cbegin() const { return this->begin(); }
        auto begin() { return this->minimum(); }

        auto rbegin() const {
            return ReverseConstIterator(this->maximum());
        }
        auto crbegin() const { return this->rbegin(); }
        auto rbegin() {
            return ReverseIterator(this->maximum());
        }

        auto end() const {
            return ConstIterator(Node::Nil::get());
        }
        auto cend() const { return this->end(); }
        auto end() {
            return Iterator(Node::Nil::get());
        }
    };
}

#endif //TPFINALAYDAI_ALGOR_RBTREE_HPP
