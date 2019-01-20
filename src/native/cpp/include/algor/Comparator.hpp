#ifndef TPFINALAYDAI_ALGOR_COMPARATOR_HPP
#define TPFINALAYDAI_ALGOR_COMPARATOR_HPP

#include <functional>
#include <algor/ComparisonResult.hpp>

namespace algor {
    template <typename T>
    class Comparator final {
    public:
        typedef ComparisonResult Result;

        typedef std::function<Result(const T &, const T &)> ComparatorFunction;

    private:
        ComparatorFunction comparatorFunction = [](const T & lhs, const T & rhs) -> Result {
            return (lhs < rhs) ? LESS : ((rhs < lhs) ? GREATER : EQUAL);
        };

    public:
        Comparator(ComparatorFunction comparatorFunction)
            : comparatorFunction(std::move(comparatorFunction))
        {}
        Comparator &operator=(ComparatorFunction const& rhs) {
            if(this != &rhs) {
                this->comparatorFunction = rhs;
            }

            return *this;
        }
        Comparator &operator=(ComparatorFunction && rhs) noexcept {
            if(this != &rhs) {
                this->comparatorFunction = std::move(rhs);
            }

            return *this;
        }

        Comparator() = default;
        Comparator(Comparator const&) = default;
        Comparator(Comparator &&) noexcept = default;
        Comparator &operator=(Comparator const&) = default;
        Comparator &operator=(Comparator &&) noexcept = default;
        ~Comparator() = default;

        Result compare(const T & lhs, const T & rhs) const {
            return this->comparatorFunction(lhs, rhs);
        }

        ComparatorFunction getComparatorFunction() const {
            return this->comparatorFunction;
        }
    };
}

#endif //TPFINALAYDAI_ALGOR_COMPARATOR_HPP
