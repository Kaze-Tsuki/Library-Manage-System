#ifndef mysort
#define mysort

#include <iostream>

template <typename RandomIt, typename Compare>
void msort(RandomIt first, RandomIt last, Compare comp) {
    if (last - first > 1) {
        RandomIt middle = first + (last - first) / 2;
        msort(first, middle, comp);
        msort(middle, last, comp);
        std::inplace_merge(first, middle, last, comp);
    }
}

template <typename Iterator, typename Func>
void my_for_each(Iterator begin, Iterator end, Func f) {
    for (auto it = begin; it != end; ++it) {
        f(*it);
    }
}

template <typename Iterator, typename Predicate>
Iterator my_find_if(Iterator begin, Iterator end, Predicate pred) {
    for (auto it = begin; it != end; ++it) {
        if (pred(*it)) return it;
    }
    return end;
}

template <typename Iterator, typename T>
Iterator my_find(Iterator begin, Iterator end, const T& value) {
    for (auto it = begin; it != end; ++it) {
        if (*it == value) return it;
    }
    return end;
}

#endif // !mysort

