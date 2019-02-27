#ifndef TPFINALAYDAI_ALGOR_UNARY_COMPARATOR_HPP
#define TPFINALAYDAI_ALGOR_UNARY_COMPARATOR_HPP

#include <functional>
#include <algor/ComparisonResult.hpp>

namespace algor {
    template <typename T>
    class UnaryComparator final {
    public:
        typedef ComparisonResult Result;

        typedef std::function<Result(const T &)> ComparatorFunction;

    private:
        ComparatorFunction comparatorFunction;

    public:
        UnaryComparator(ComparatorFunction comparatorFunction)
            : comparatorFunction(std::move(comparatorFunction))
        {}
        UnaryComparator &operator=(ComparatorFunction const& rhs) {
            if(this != &rhs) {
                this->comparatorFunction = rhs;
            }

            return *this;
        }
        UnaryComparator &operator=(ComparatorFunction && rhs) noexcept {
            if(this != &rhs) {
                this->comparatorFunction = std::move(rhs);
            }

            return *this;
        }

        UnaryComparator() = delete;
        UnaryComparator(UnaryComparator const&) = default;
        UnaryComparator(UnaryComparator &&) noexcept = default;
        UnaryComparator &operator=(UnaryComparator const&) = default;
        UnaryComparator &operator=(UnaryComparator &&) noexcept = default;
        ~UnaryComparator() = default;

        Result compare(const T & lhs) const {
            return this->comparatorFunction(lhs);
        }

        ComparatorFunction getComparatorFunction() const {
            return this->comparatorFunction;
        }
    };
}

#endif //TPFINALAYDAI_ALGOR_UNARY_COMPARATOR_HPP
