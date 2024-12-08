#pragma once

#include <iterator>
#include <functional>


template <
    typename RandomIt,
    typename Compare
>
void heapify(RandomIt begin, int index, size_t size, Compare comp);


template <
    typename RandomIt,
    typename Compare
>
void makeHeap(RandomIt begin, RandomIt end, Compare comp);


template <
    typename RandomIt,
    typename Compare
>
void popHeap(RandomIt begin, RandomIt end, Compare comp);


template <
    typename RandomIt,
    typename Compare
>
void pushHeap(RandomIt begin, RandomIt end, Compare comp);



template <
    typename RandomIt, 
    typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>
>
void heapify(RandomIt begin, int index, size_t size, Compare comp=Compare()) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && comp(*(begin + largest), *(begin + left))) {
        largest = left;
    }
    if (right < size && comp(*(begin + largest), *(begin + right))) {
        largest = right;
    }
    if (largest != index) {
        std::swap(*(begin + index), *(begin + largest));
        heapify(begin, largest, size, comp);
    }
}


template <
    typename RandomIt,
    typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>
>
void makeHeap(RandomIt begin, RandomIt end, Compare comp=Compare()) {
    size_t size = std::distance(begin, end);
    if (size <= 1) return;
    for (int i = std::floor(size / 2) - 1; i >= 0; --i) {
        heapify(begin, i, size, comp);
    }
}


template <
    typename RandomIt,
    typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>
>
void popHeap(RandomIt begin, RandomIt end, Compare comp=Compare()) {
    size_t size = std::distance(begin, end);
    if (size <= 1) return;
    std::iter_swap(begin, end - 1);
    heapify(begin, 0, size - 1, comp);
}


template <
    typename RandomIt,
    typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>
>
void pushHeap(RandomIt begin, RandomIt end, Compare comp = Compare()) {
    size_t size = std::distance(begin, end);
    if (size <= 1) return;
    int newIndex = size - 1;
    while (newIndex > 0) {
        int parentIndex = (newIndex-1) / 2;
        auto& parentEl = *(begin + parentIndex);
        auto& newEl = *(begin + newIndex);
        if (comp(parentEl, newEl)) {
            std::swap(parentEl, newEl);
            newIndex = parentIndex;
        }
        else {
            break;
        }
    }
}
