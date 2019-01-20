#ifndef TPFINALAYDAI_LIST_IMPL_NODE_HPP
#define TPFINALAYDAI_LIST_IMPL_NODE_HPP

namespace algor::__detail__List {
    template <typename T>
    struct Node {
        T elem;
        Node *next;

        Node * clone(Node * & last) {
            Node * new_node = new Node{this->elem, nullptr};

            if(this->next != nullptr) {
                new_node->next = this->next->clone(last);
            } else {
                last = new_node;
            }

            return new_node;
        }
    };
}

#endif //TPFINALAYDAI_LIST_IMPL_NODE_HPP
