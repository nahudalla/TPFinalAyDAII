#include "../Test.hpp"

#include <algor/RBTree.hpp>

typedef algor::RBTree<int> RBTree;

bool rbtree_insert_and_size(); ADD_TEST(RBTREE_INSERT_AND_SIZE, rbtree_insert_and_size);
bool rbtree_iterate(); ADD_TEST(RBTREE_ITERATE, rbtree_iterate);
bool rbtree_iterate_backwards(); ADD_TEST(RBTREE_ITERATE_BACKWARDS, rbtree_iterate_backwards);
bool rbtree_minimum(); ADD_TEST(RBTREE_MINIMUM, rbtree_minimum);
bool rbtree_maximum(); ADD_TEST(RBTREE_MAXIMUM, rbtree_maximum);
bool rbtree_search(); ADD_TEST(RBTREE_SEARCH, rbtree_search);
bool rbtree_remove(); ADD_TEST(RBTREE_REMOVE, rbtree_remove);
bool rbtree_copy(); ADD_TEST(RBTREE_COPY, rbtree_copy);

RBTree create_tree(int size, int min = 0);

bool rbtree_insert_and_size() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);

        if(tree.size() != i) return false;
    }

    return true;
}

bool rbtree_iterate() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);

        auto it = tree.begin();
        auto it_const = tree.cbegin();

        for (int j = 0; j < i; ++j) {
            if(*it != j || *it_const != j ||
                *it != *it_const || it != it_const) return false;

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

        if(it!=tree.end() || it_const!=tree.cend()) return false;
    }

    return true;
}

bool rbtree_iterate_backwards() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);

        auto it = tree.rbegin();
        auto it_const = tree.crbegin();

        for (int j = i-1; j >= 0; --j) {
            if(*it != j || *it_const != j ||
                *it != *it_const || it != it_const) return false;

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

        if(it != tree.end() || it_const != tree.cend()) return false;
    }

    return true;
}

bool rbtree_minimum() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i, i);

        auto min = tree.minimum();

        if((i == 0 && min != tree.end()) || (i != 0 && *min != i)) return false;
    }

    return true;
}

bool rbtree_maximum() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);

        auto max = tree.maximum();

        if((i == 0 && max != tree.end()) || (i != 0 && *max != i-1)) return false;
    }

    return true;
}

bool rbtree_search() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);
        auto end = tree.end();

        for (int j = 0; j < i; ++j) {
            auto elem = tree.search(j);

            if(elem == end || *elem != j) return false;
        }
    }

    return true;
}

bool rbtree_remove() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);

        auto end = tree.end();

        for (int j = i-1; j >= 0; --j) {
            tree.remove(j);

            if(tree.search(j) != end) return false;
        }

        if(tree.size() != 0) return false;
    }

    return true;
}

bool rbtree_copy() {
    for (int i = 0; i < 20; ++i) {
        auto tree = create_tree(i);
        RBTree copy(tree);

        auto end = tree.end();

        for (int j = 0; j < i; ++j) {
            copy.remove(j);

            if(tree.search(j) == end) return false;
        }
    }

    return true;
}

RBTree create_tree(int size, int min) {
    RBTree tree;

    for (int i = min; i < size+min; ++i) {
        tree.insert(i);
    }

    return std::move(tree);
}
