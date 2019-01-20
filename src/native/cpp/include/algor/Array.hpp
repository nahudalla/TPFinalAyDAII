#ifndef TPFINALAYDAI_ARRAY_HPP
#define TPFINALAYDAI_ARRAY_HPP

#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <algor/Comparator.hpp>
#include <algor/List.hpp>

namespace algor {
    template <typename T>
    class Array {
        T * data = nullptr;
        std::size_t _size = 0;

        void init(std::size_t size) {
            this->_size = size;
            this->data = new T[size];
        }
        void init(std::initializer_list<T> list) {
            this->init(list.size());
            auto it = list.begin();
            auto end = list.end();
            for(std::size_t i = 0; it != end; ++it, ++i) {
                this->data[i] = std::move(*it);
            }
        }
    public:
        explicit Array(std::size_t size) {this->init(size);}
        Array(std::initializer_list<T> list) {*this=std::move(list);}
        Array &operator=(std::initializer_list<T> list) {
            this->~Array();
            this->init(std::move(list));

            return *this;
        }

        explicit Array(List<T> const& list) {*this = list;}
        Array &operator=(List<T> const& list) {
            this->~Array();
            this->init(list.length());

            auto it = list.begin();
            auto end = list.end();
            for(std::size_t i = 0; it != end; ++it, ++i) {
                this->data[i] = *it;
            }

            return *this;
        }

        explicit Array(List<T> && list) {*this = std::move(list);}
        Array &operator=(List<T> && list) {
            this->~Array();
            this->init(list.length());

            for(std::size_t i = 0; !list.isEmpty(); ++i) {
                this->data[i] = list.remove(list.begin());
            }

            return *this;
        }

        Array(Array const& other) {*this=other;}
        Array(Array && other) noexcept {*this=std::move(other);}
        ~Array() {
            delete[] this->data;
            this->data = nullptr;
            this->_size = 0;
        }

        Array &operator=(Array const& other) {
            if(this != &other) {
                this->~Array();
                this->init(other._size);
                for (int i = 0; i < other._size; ++i) {
                    this->data[i] = other.data[i];
                }
            }

            return *this;
        }
        Array &operator=(Array && other) noexcept {
            if(this != &other) {
                this->~Array();
                std::swap(this->data, other.data);
                std::swap(this->_size, other._size);
            }

            return *this;
        }

        std::size_t size() const {
            return this->_size;
        }

        T &operator[](std::size_t i) {
            return this->data[i];
        }
        const T&operator[](std::size_t i) const {
            return this->data[i];
        }

        T * raw() {
            return this->data;
        }

        const T * raw() const {
            return this->data;
        }

        void sort(Comparator<T> cmp = Comparator<T>()) {
            std::sort(this->data, this->data+this->_size, [&cmp](const T & lhs, const T & rhs) {
                return cmp.getComparatorFunction()(lhs, rhs) == Comparator<T>::Result::LESS;
            });
        }
    };
}

#endif //TPFINALAYDAI_ARRAY_HPP
