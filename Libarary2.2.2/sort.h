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


#endif // !mysort

