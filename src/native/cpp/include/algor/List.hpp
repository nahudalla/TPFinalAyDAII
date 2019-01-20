#ifndef TPFINALAYDAI_ALGOR_LIST_HPP
#define TPFINALAYDAI_ALGOR_LIST_HPP

#include <algor/List_impl/Node.hpp>
#include <algor/List_impl/ConstIterator.hpp>
#include <algor/List_impl/Iterator.hpp>

#include <algor/Comparator.hpp>

#include <functional>
#include <initializer_list>

#ifndef __EMSCRIPTEN__
#include <ostream>
#endif

namespace algor {
    template<typename T>
    class List {
        typedef __detail__List::Node<T> Node;

    public:
        typedef __detail__List::Iterator<T> Iterator;
        typedef __detail__List::ConstIterator<T> ConstIterator;

    private:
        Node * first = nullptr;
        Node * last = nullptr;

        void merge(Comparator<T> const& cmp, Iterator & a_begin, Iterator b, Iterator & end) {
            // IMPORTANTE: Los iteradores a_begin y end DEBEN ser actualizados para que apunten
            // a los nuevos nodos de inicio y fin de la lista ordenada

            auto a = a_begin;

            while(a != b && b != end) {
                if(cmp.compare(*a, *b) == Comparator<T>::Result::GREATER) {
                    // Incremento b antes de modificarlo para no perder el siguiente
                    auto b_next = b; b_next.next();

                    // Saco b de su lugar
                    b.prev->next = b.curr->next;

                    // Actualizo iteradores
                    b_next.prev = b.prev;
                    if(b_next == end) end.prev = b.prev;

                    // Si b es el último, actualizo el puntero al último elemento de la lista
                    if(b.curr->next == nullptr) {
                        this->last = b.prev;
                    }

                    // Pongo b antes que a
                    if(a.prev == nullptr) {
                        this->first = b.curr;
                    } else {
                        a.prev->next = b.curr;
                    }
                    b.curr->next = a.curr;

                    // Actualizo iteradores
                    b.prev = a.prev;
                    if(a == a_begin) {
                        a_begin = b;
                    }
                    a.prev = b.curr;

                    // Incremento el iterador de b
                    b = b_next;
                } else {
                    a.next();
                }
            }
        }

        Iterator find_middle(Iterator begin, Iterator const& end) {
            auto slow = std::move(begin);
            auto fast = slow; fast.next();

            while(fast != end) {
                fast.next();
                if(fast != end) {
                    slow.next();
                    fast.next();
                }
            }

            slow.next();

            return std::move(slow);
        }

        void merge_sort(Comparator<T> const& cmp, Iterator & begin, Iterator & end) {
            // Si hay menos de dos elementos, ya está ordenada
            {
                Iterator next;
                if (begin == end || ((next = begin), next.next()), next == end) return;
            }

            // Split (Divide)
            Iterator a_end = this->find_middle(begin, end);

            // Sort
            this->merge_sort(cmp, begin, a_end);
            this->merge_sort(cmp, a_end, end);

            // Merge (Conquer)
            this->merge(cmp, begin, a_end, end);
        }
    public:
        List() = default;

        List(List const& other) {*this = other;}
        List &operator=(List const& other) {
            if(this != &other) {
                this->~List();
                if(other.first != nullptr) {
                    this->first = other.first->clone(this->last);
                }
            }

            return *this;
        }
        List(List && other) noexcept {*this = std::move(other);}
        List &operator=(List && rhs) noexcept {
            if(this != &rhs) {
                std::swap(this->first, rhs.first);
                std::swap(this->last, rhs.last);
            }

            return *this;
        }
        ~List() {
            while (this->first != nullptr) {
                auto *tmp = this->first;
                this->first = this->first->next;
                delete tmp;
            }
            this->last = nullptr;
        }

        List(std::initializer_list<T> list) {*this = std::move(list);}
        List &operator=(std::initializer_list<T> list) {
            auto it = list.begin();
            auto end = list.end();
            for(; it != end; ++it) {
                this->add(std::move(*it));
            }

            return *this;
        }

        void add(T elem) {
            // Creo un nuevo nodo
            auto * node = new Node{std::move(elem), nullptr};

            // Si no hay último elemento, la lista está vacía
            if (this->last == nullptr) {
                // Inicializo la lista con el nuevo nodo
                this->first = node;
                this->last = node;
            } else {
                // La lista tiene elementos, lo agrego al final
                this->last->next = node;
                // Pongo el elemento nuevo como el ultimo
                this->last = node;
            }
        }

        void add_front(T elem) {
            this->first = new Node{std::move(elem), this->first};

            if(this->last == nullptr) this->last = this->first;
        }

        std::size_t length() const {
            Node * current = this->first; // Inicializo el nodo actual como el primero
            std::size_t counter = 0; // Inicializo el contador en cero

            while (current != nullptr) { // Mientras que haya nodo actual
                counter++; // Aumento el contador
                current = current->next; // Me muevo al siguiente
            }

            return counter;
        }

        auto begin() {
            return Iterator(nullptr, this->first);
        }
        auto begin() const {
            return ConstIterator(nullptr, this->first);
        }
        auto cbegin() const { return this->begin(); }

        auto end() {
            return Iterator(nullptr, nullptr);
        }
        auto end() const {
            return ConstIterator(nullptr, nullptr);
        }
        auto cend() const { return this->end(); }

        T remove(Iterator const& it) {
            auto next = it.curr->next;

            if (it.prev != nullptr) it.prev->next = next;

            T elem = std::move(it.curr->elem);

            delete it.curr;

            if (this->first == it.curr) this->first = next;
            if (this->last == it.curr) this->last = it.prev;

            return std::move(elem);
        }

        bool isEmpty() const {
            return this->first == nullptr && this->last == nullptr;
        }

        void swap(Iterator const& e1, Iterator const& e2) {
            if (!e1 || !e2 || e1 == e2) return;

            auto e1_prev = e1.prev;
            auto e1_curr = e1.curr;
            auto e2_prev = e2.prev;
            auto e2_curr = e2.curr;

            if (e1_curr->next == nullptr) this->last = e2_curr;
            if (e2_curr->next == nullptr) this->last = e1_curr;

            if (e1_prev != nullptr) e1_prev->next = e2_curr;
            else this->first = e2_curr;

            if (e2_prev != nullptr) e2_prev->next = e1_curr;
            else this->first = e1_curr;

            auto tmp = e1_curr->next;
            e1_curr->next = e2_curr->next;
            e2_curr->next = tmp;
        }

        void sort(Comparator<T> const& cmp = Comparator<T>()) {
            auto b = this->begin();
            auto e = this->end();
            this->merge_sort(cmp, b, e);
        }

        auto findMinimum(Comparator<T> const& cmp = Comparator<T>()) const {
            auto min = this->begin();
            auto it = min; ++it;
            auto end = this->end();

            for(;it != end; ++it) {
                if(cmp.compare(*it, *min) == LESS) {
                    min = it;
                }
            }

            return min;
        }
        auto findMinimum(Comparator<T> const& cmp = Comparator<T>()) {
            return Iterator(const_cast<const List *>(this)->findMinimum(cmp));
        }

        void removeIf(std::function<bool(const Iterator&, const Iterator&)> const& condition) {
            auto it_prev = this->end();
            auto it = this->begin();
            auto end = this->end();

            for(; it != end; ++it) {
                if(condition(it, end)) this->remove(it);

                if(it_prev == end) {
                    it_prev = this->begin();
                    it = it_prev;
                } else {
                    it_prev++;
                    it = it_prev;
                }
            }
        }

        bool operator==(const List & rhs) {
            if(this == &rhs) return true;

            auto lhs_it = this->begin();
            auto lhs_end = this->end();
            auto rhs_it = rhs.begin();
            auto rhs_end = rhs.end();

            for(; lhs_it != lhs_end && rhs_it != rhs_end; ++lhs_it, ++rhs_it) {
                if(!(*lhs_it == *rhs_it)) return false;
            }

            if(lhs_it != lhs_end || rhs_it != rhs_end) {
                return false;
            }

            return true;
        }

        bool operator!=(const List & rhs) {
            return !(*this == rhs);
        }

#ifndef __EMSCRIPTEN__
        friend std::ostream &operator<<(std::ostream & out, List const& list) {
            auto it = list.begin();
            auto end = list.end();
            auto first = list.begin();

            for(; it != end; ++it) {
                if(it != first) out << std::endl;
                out << *it;
            }

            return out;
        }
#endif
    };
}

#endif //TPFINALAYDAI_ALGOR_LIST_HPP
