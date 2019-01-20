#include "../Test.hpp"

#include <algor/Stack.hpp>

using namespace algor;

bool stack_create_and_push(); ADD_TEST(STACK_CREATE_AND_PUSH, stack_create_and_push);
bool stack_pop_and_empty(); ADD_TEST(STACK_POP_AND_EMPTY, stack_pop_and_empty);
bool stack_top_and_next_to_top(); ADD_TEST(STACK_TOP_AND_NEXT_TO_TOP, stack_top_and_next_to_top);

Stack<int> create_stack(int cant);

bool stack_create_and_push() {
    for (int i = 0; i < 10; ++i) {
        auto s = create_stack(i);
    }

    return true;
}

bool stack_pop_and_empty() {
    for (int i = 0; i < 10; ++i) {
        auto s = create_stack(i);

        for (int j = 0; j < i; ++j) {
            if(s.isEmpty()
                || (j < i-1 && !s.hasNextToTop())
                || (j == i-1 && s.hasNextToTop())) {
                return false;
            }

            s.pop();
        }

        if(!s.isEmpty()) return false;
    }

    return true;
}

bool stack_top_and_next_to_top() {
    for (int i = 0; i < 10; ++i) {
        auto s = create_stack(i);

        for (int j = 0; j < i; ++j) {
            if(s.top() != j || (j < i-1 && s.nextToTop() != j+1)) {
                return false;
            }

            s.pop();
        }
    }

    return true;
}

Stack<int> create_stack(int cant) {
    Stack<int> stack;

    for (int i = cant-1; i >= 0; --i) {
        stack.push(i);
    }

    return std::move(stack);
}
