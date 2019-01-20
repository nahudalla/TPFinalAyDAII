#ifndef TPFINALAYDAI_RBTREE_IMPL_NODE_HPP
#define TPFINALAYDAI_RBTREE_IMPL_NODE_HPP

#include <algorithm>

#include <algor/RBTree_impl/Color.hpp>
#include <algor/Comparator.hpp>

namespace algor::__detail_RBTree {
    template <typename> class Nil;

    template <typename T>
    class Node {
        Node * left = nullptr, * right = nullptr, * parent = nullptr;
        Color color = BLACK;

        void rotate_left(Node * & root) {
            Node * y = this->right;

            this->right = y->left;

            if(!y->left->isNil()) {
                y->left->parent = this;
            }

            y->parent = this->parent;

            if(this->parent->isNil()) {
                root = y;
            } else if(this == this->parent->left) {
                this->parent->left = y;
            } else {
                this->parent->right = y;
            }

            y->left= this;
            this->parent = y;
        }
        void rotate_right(Node * & root) {
            Node * y = this->left;

            this->left= y->right;

            if(!y->right->isNil()) {
                y->right->parent = this;
            }

            y->parent = this->parent;

            if(this->parent->isNil()) {
                root = y;
            } else if(this == this->parent->left) {
                this->parent->left = y;
            } else {
                this->parent->right = y;
            }

            y->right= this;
            this->parent = y;
        }
        void insert_fixup(Node * & root) {
            Node * z = this;
            while(z->parent->color == RED) {
                if(z->parent == z->parent->parent->left) {
                    Node * y = z->parent->parent->right;
                    if(y->color == RED) {
                        z->parent->color = BLACK;
                        y->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    } else {
                        if(z == z->parent->right) {
                            z = z->parent;
                            z->rotate_left(root);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        z->parent->parent->rotate_right(root);
                    }
                } else {
                    Node * y = z->parent->parent->left;
                    if(y->color == RED) {
                        z->parent->color = BLACK;
                        y->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    } else {
                        if(z == z->parent->left) {
                            z = z->parent;
                            z->rotate_right(root);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        z->parent->parent->rotate_left(root);
                    }
                }
            }

            root->color = BLACK;
        }
        void transplant_to(Node * & root, Node * to) {
            if(to->parent->isNil()) {
                root = this;
            } else if(to == to->parent->left) {
                to->parent->left = this;
            } else to->parent->right = this;

            this->parent = to->parent;
        }
        void remove_fixup(Node * & root) {
            Node * x = this;
            while(x != root && x->color == BLACK) {
                if(x == x->parent->left) {
                    Node * w = x->parent->right;
                    if(w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        x->parent->rotate_left(root);
                        w = x->parent->right;
                    }
                    if(w->left->color == BLACK && w->right->color == BLACK) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if(w->right->color == BLACK) {
                            w->left->color = BLACK;
                            w->color = RED;
                            w->rotate_right(root);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->right->color = BLACK;
                        x->parent->rotate_left(root);
                        x = root;
                    }
                } else {
                    Node * w = x->parent->left;
                    if(w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        x->parent->rotate_right(root);
                        w = x->parent->left;
                    }
                    if(w->right->color == BLACK && w->left->color == BLACK) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if(w->left->color == BLACK) {
                            w->right->color = BLACK;
                            w->color = RED;
                            w->rotate_left(root);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->left->color = BLACK;
                        x->parent->rotate_right(root);
                        x = root;
                    }
                }
            }
            x->color = BLACK;
        }
    public:
        typedef Nil<T> Nil;

        const Node* to_const() const { return this; }
        Node* to_non_const() const { return const_cast<Node*>(this); }

        Node(Node * parent, Node * left, Node * right, Color color = BLACK)
                : parent(parent), left(left), right(right), color(color)
        {}

        Node() : parent(Nil::get()), left(Nil::get()), right(Nil::get())
        {}

        Node(Node const& other) {*this = other;}
        Node(Node && other) noexcept {*this = std::move(other);}
        Node &operator=(Node const& other) {
            if(this != &other) {
                this->~Node();
                this->left = other.left->clone();
                this->right = other.right->clone();
                this->left->parent = this;
                this->right->parent = this;
                this->color = other.color;
            }
            return *this;
        }
        Node &operator=(Node && rhs) noexcept {
            if(this != rhs) {
                std::swap(this->left, rhs.left);
                std::swap(this->right, rhs.right);

                this->left->parent = this;
                this->right->parent = this;

                this->parent = rhs.parent;
                this->color = rhs.color;

                rhs.left->parent = &rhs;
                rhs.right->parent = &rhs;
            }
            return *this;
        }
        virtual ~Node() {
            if(this->left != nullptr && !this->left->isNil()) {
                delete this->left;
                this->left = Nil::get();
            }
            if(this->right != nullptr && !this->right->isNil()) {
                delete this->right;
                this->right = Nil::get();
            }
            this->parent = Nil::get();
        }

        std::size_t size() const {
            if(this->isNil()) return 0;

            return 1 + this->left->size() + this->right->size();
        }

        virtual const T * getData() const = 0;
        virtual T * getData() = 0;
        virtual Node * clone() const = 0;

        void setLeft(Node *left) { this->left = left; }
        void setRight(Node *right) { this->right = right; }
        void setParent(Node *parent) { this->parent = parent; }
        void setColor(Color color) { this->color = color; }

        bool isNil() const { return this == Nil::get(); }

        const Node * search(T const& data, const Comparator<T> & comparator) const {
            const Node * current = this;
            const T * current_data;

            while (!current->isNil() && !(*(current_data = current->getData()) == data)) {
                if(comparator.compare(*current_data, data) == LESS) current = current->right;
                else current = current->left;
            }

            return current;
        }
        Node * search(T const& data, const Comparator<T> & comparator) {
            return this->to_const()->search(data, comparator)->to_non_const();
        }

        const Node * minimum() const {
            const Node * min = this;

            while(!min->left->isNil()) min = min->left->minimum();

            return min;
        }
        Node * minimum() {
            return this->to_const()->minimum()->to_non_const();
        }

        const Node * maximum() const {
            const Node * max = this;

            while(!max->right->isNil()) max = max->right->maximum();

            return max;
        }
        Node * maximum() {
            return this->to_const()->maximum()->to_non_const();
        }

        const Node * next() const {
            if(!this->right->isNil()) {
                return this->right->minimum();
            }

            const Node * current = this;
            const Node * successor = this->parent;

            while (!successor->isNil() && current == successor->right) {
                current = successor;
                successor = successor->parent;
            }

            return successor;
        }
        Node * next() {
            return this->to_const()->next()->to_non_const();
        }

        const Node * previous() const {
            if(!this->left->isNil()) {
                return this->left->maximum();
            }

            const Node * current = this;
            const Node * predecessor = this->parent;

            while (!predecessor->isNil() && current == predecessor->left) {
                current = predecessor;
                predecessor = predecessor->parent;
            }

            return predecessor;
        }
        Node * previous() {
            return this->to_const()->previous()->to_non_const();
        }

        void insert(Node * & root, const Comparator<T> & comparator) {
            Node * y = Node::Nil::get();
            Node * x = root;

            while(!x->isNil()) {
                y = x;
                if(comparator.compare(*(this->getData()), *(x->getData())) == LESS) x = x->left;
                else x = x->right;
            }

            this->parent = y;

            if(y->isNil()) root = this;
            else if(comparator.compare(*(this->getData()), *(y->getData())) == LESS) y->left = this;
            else y->right = this;

            this->left = Node::Nil::get();
            this->right = Node::Nil::get();
            this->color = RED;

            this->insert_fixup(root);
        }

        void remove(Node * & root) {
            Node * z = this;
            Node * y = z;
            Node * x;
            auto y_original_color = y->color;

            if(z->left->isNil()) {
                x = z->right;
                z->right->transplant_to(root, z);
            } else if(z->right->isNil()) {
                x = z->left;
                z->left->transplant_to(root, z);
            } else {
                y = z->right->minimum();
                y_original_color = y->color;
                x = y->right;
                if(y->parent == z) {
                    x->parent = y;
                } else {
                    y->right->transplant_to(root, y);
                    y->right = z->right;
                    y->right->parent = y;
                }
                y->transplant_to(root, z);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
            if(y_original_color == BLACK) {
                x->remove_fixup(root);
            }
        }
    };
}

#include <algor/RBTree_impl/Nil.hpp>

#endif //TPFINALAYDAI_RBTREE_IMPL_NODE_HPP
