#include "../Test.hpp"

#include <algor/List.hpp>
#include <algor/Comparator.hpp>
#include <random>

using namespace algor;

bool list_create(); ADD_TEST(LIST_CREATE, list_create);
bool list_empty(); ADD_TEST(LIST_EMPTY, list_empty);
bool list_length(); ADD_TEST(LIST_LENGTH, list_length);
bool list_iterator(); ADD_TEST(LIST_ITERATOR, list_iterator);
bool list_iterator_basic_equality(); ADD_TEST(LIST_ITERATOR_BASIC_EQUALITY, list_iterator_basic_equality);
bool list_iterator_copy(); ADD_TEST(LIST_ITERATOR_COPY, list_iterator_copy);
bool list_iterator_equality(); ADD_TEST(LIST_ITERATOR_EQUALITY, list_iterator_equality);
bool list_modify_element(); ADD_TEST(LIST_MODIFY_ELEMENT, list_modify_element);
bool list_remove_element(); ADD_TEST(LIST_REMOVE_ELEMENT, list_remove_element);
bool list_remove_if(); ADD_TEST(LIST_REMOVE_IF, list_remove_if);
bool list_swap(); ADD_TEST(LIST_SWAP, list_swap);
bool list_sort(); ADD_TEST(LIST_SORT, list_sort);
bool list_copy(); ADD_TEST(LIST_COPY, list_copy);
bool list_find_minimum(); ADD_TEST(LIST_FIND_MINIMUM, list_find_minimum);
bool list_add_front(); ADD_TEST(LIST_ADD_FRONT, list_add_front);

List<int> create_list(int cant);
bool is_complete_and_ordered(List<int> &list, int cant);
void shuffle_list(List<int> & list, int cant);

bool list_create() {
    for (int i = 0; i < 10; ++i) {
        create_list(i);
    }

    return true;
}

bool list_empty() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);

        if(list.isEmpty() && i != 0) return false;
    }

    return true;
}

bool list_length() {
    for (int i = 0; i < 10; ++i) {
        List<int> list = create_list(i);

        const auto l = list.length();

        if(l != i) return false;
    }

    return true;
}

bool list_iterator() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);

        auto it = list.begin();
        auto it_const = list.cbegin();

        for (int j = 0; j < i; ++j) {
            if(!it.isDereferentiable() || *it != j) return false;

            if(j%3 == 0) {
                it.next();
                it_const.next();
            } else if(j%2 == 0) {
                it++;
                it_const++;
            } else {
                ++it;
                ++it_const;
            }
        }

        if(it.isDereferentiable()) return false;
    }

    return true;
}

bool list_iterator_basic_equality() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);
        auto it = list.begin();
        auto it2 = list.begin();
        for (int j = 0; j < i; ++j) {
            if(it != it2) return false;
            it.next();
            it2.next();
        }
        if(it != list.end()) return false;
    }

    return true;
}

bool list_iterator_copy() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);

        auto it = list.begin();
        auto end = list.end();

        decltype(it) it2;

        while(it != end) {
            it2 = it;
            if(it2 != it) return false;
            it.next();
        }
    }
    return true;
}

bool list_iterator_equality() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);
        auto it = list.begin();

        if(i != 0) {
            List<int>::Iterator iterators[i];
            for (int j = 0; j < i; ++j) {
                iterators[j] = it;
                it.next();
            }

            it = list.begin();
            auto end = list.end();
            int count = 0;
            while(it != end) {
                for (int k = 0; k < i; ++k) {
                    if (count == k) {
                        if (iterators[k] != it) return false;
                    } else {
                        if (iterators[k] == it) return false;
                    }
                }
                it.next();
                count++;
            }
        }

        if(it != list.end()) return false;
    }

    return true;
}

bool list_modify_element() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);
        auto it = list.begin();
        auto end = list.end();

        while(it != end) {
            *it += 7;
            it.next();
        }

        it = list.begin();
        for (int j = 0; j < i; ++j, it.next()) {
            if(*it != 7+j) return false;
        }
    }

    return true;
}

bool list_remove_element() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);
        auto it = list.begin();

        for (int j = 0; j < i; ++j) {
            if(j%2 == 0) {
                auto it2 = it;
                it2.next();

                list.remove(it);

                it = it2;
            } else it.next();
        }

        it = list.begin();
        auto end = list.end();
        int num = 1;
        while(it != end) {
            if(*it != num) return false;
            it.next();
            num += 2;
        }
    }

    return true;
}

bool list_remove_if() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);

        list.removeIf([](const List<int>::Iterator & _it, const List<int>::Iterator &) -> bool {
            auto it = _it;
            return (*it)%2 == 0;
        });

        auto it = list.begin();
        auto end = list.end();
        int num = 1;
        while(it != end) {
            if(*it != num) return false;
            it.next();
            num += 2;
        }
    }

    return true;
}

bool list_swap() {
    for (int i = 0; i < 10; ++i) {
        auto list = create_list(i);
        auto it = list.begin();
        auto end = list.end();
        auto it_prev = end;

        while(it != end) {
            auto next = it; next.next();
            list.swap(it, next);

            if(it_prev == end) {
                it_prev = list.begin().next();
            } else {
                it_prev.next().next();
            }

            it = it_prev; it.next();
        }

        it = list.begin();
        for (int j = 0; j < (i%2==0?i:i-1); ++j, it.next()) {
            if(j%2 == 0) {
                if(*it != j+1) return false;
            } else {
                if(*it != j-1) return false;
            }
        }
    }

    return true;
}

bool list_sort() {
    std::random_device rd;
    std::mt19937 mt(rd());

    for (int i = 2; i < 10; ++i) {
        auto list = create_list(i);

        shuffle_list(list, i);

        // Third, sort the list
        list.sort();

        if(!is_complete_and_ordered(list, i)) return false;
    }

    return true;
}

bool list_find_minimum() {
    typedef Comparator<int>::ComparatorFunction CmpFn;
    typedef Comparator<int>::Result CmpRes;

    auto cmp1 = (CmpFn) [](const int & lhs, const int & rhs) -> CmpRes {
        return lhs > rhs ? CmpRes::LESS
                         : (rhs > lhs ? CmpRes::GREATER : CmpRes::EQUAL);
    };

    auto cmp2 = (CmpFn) [](const int & lhs, const int & rhs) -> CmpRes {
        if(lhs == 0 && rhs == 0) return CmpRes::EQUAL;
        else if(lhs == 0) return CmpRes::GREATER;
        else if(rhs == 0) return CmpRes::LESS;

        if(lhs%2 != 0 && rhs%2 != 0) return CmpRes::EQUAL;
        else if(lhs%2 != 0) return CmpRes::GREATER;
        else if(rhs%2 != 0) return CmpRes::LESS;

        return lhs < rhs ? CmpRes::LESS
                         : (rhs < lhs ? CmpRes::GREATER : CmpRes::EQUAL);
    };

    auto cmp3 = (CmpFn) [](const int & lhs, const int & rhs) -> CmpRes {
        if(lhs == 0 && rhs == 0) return CmpRes::EQUAL;
        else if(lhs == 0) return CmpRes::GREATER;
        else if(rhs == 0) return CmpRes::LESS;

        if(lhs%2 != 0 && rhs%2 != 0) return CmpRes::EQUAL;
        else if(lhs%2 != 0) return CmpRes::GREATER;
        else if(rhs%2 != 0) return CmpRes::LESS;

        return lhs > rhs ? CmpRes::LESS
                         : (rhs > lhs ? CmpRes::GREATER : CmpRes::EQUAL);
    };

    for (int i = 2; i < 10; ++i) {
        auto list = create_list(i);

        shuffle_list(list, i);

        if(*(list.findMinimum()) != 0) return false;
        if(*(list.findMinimum(cmp1)) != i-1) return false;
        if(i >= 3 && *(list.findMinimum(cmp2)) != 2) return false;

        int lhs = *(list.findMinimum(cmp3));
        int rhs = ( ((i-1)%2 == 0) ? (i-1) : (i-2) );

        if(i >= 3 && lhs != rhs) return false;
    }

    return true;
}

bool list_copy() {
    {
        auto list = create_list(0);
        auto copy = list;

        copy.add(1);

        if(copy.length() != 1 || list.length() != 0) {
            return false;
        }
    }

    {
        auto list = create_list(1);
        auto copy = list;
        const auto NUMBER = 77777;

        *(copy.begin()) = NUMBER;

        if(*(copy.begin()) != NUMBER || *(list.begin()) == NUMBER) {
            return false;
        }
    }

    for (int i = 2; i < 10; ++i) {
        auto list = create_list(i);
        auto copy = list;

        auto it = copy.begin();
        auto end = copy.end();
        auto it_prev = end;
        while(it != end) {
            if(*it%2 == 0) {
                copy.remove(it);
            }

            if(it_prev == end) {
                it_prev = copy.begin();
            } else {
                ++it_prev;
            }

            it = it_prev;
            ++it;
        }

        if(is_complete_and_ordered(copy, i) ||
            !is_complete_and_ordered(list, i))
        {
            return false;
        }
    }

    return true;
}

bool list_add_front() {
    for (int i = 0; i < 10; ++i) {
        List<int> l;

        for (int j = i-1; j >= 0; --j) {
            l.add_front(j);
        }

        auto it = l.begin();
        auto end = l.end();
        for(int j = 0; it != end; ++it, ++j) {
            if(*it != j) return false;
        }
    }

    return true;
}

List<int> create_list(int cant) {
    List<int> list;

    for (int i = 0; i < cant; ++i) {
        list.add(i);
    }

    return std::move(list);
}

bool is_complete_and_ordered(List<int> &list, int cant) {
    auto it = list.begin();
    auto end = list.end();
    int i = 0;
    while(it != end) {
        if(*it != i) return false;

        it.next();
        i++;
    }

    return i == cant;
}

void shuffle_list(List<int> &list, int cant) {
    std::random_device rd;
    std::mt19937 mt(rd());

    do {
        auto it = list.begin();
        auto end = list.end();

        // First, shuffle the list
        std::uniform_int_distribution<int> dist(0, cant);
        while (it != end) {
            auto n = dist(mt);
            auto it2 = it;
            for (; n > 0; --n) it2.next();

            list.swap(it, it2);

            it.next();
        }

        // Second, make sure that it is really shuffled
    }while(is_complete_and_ordered(list, cant));
}
