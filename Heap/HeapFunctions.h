#pragma once

#include <iterator>
#include <functional>


template <class RandomIt>
void heapify(RandomIt begin, int index, size_t size);

template <class RandomIt>
void makeHeap(RandomIt begin, RandomIt end);


template <class RandomIt>
void heapify(RandomIt begin, int index, size_t size) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && *(begin + left) > *(begin + largest)) {
        largest = left;
    }
    if (right < size && *(begin + right) > *(begin + largest)) {
        largest = right;
    }
    if (largest != index) {
        std::swap(*(begin + index), *(begin + largest));
        heapify(begin, largest, size);
    }
}


template <class RandomIt>
void makeHeap(RandomIt begin, RandomIt end) {
    size_t size = std::distance(begin, end);
    if (size <= 1) return;
    for (int i = std::floor(size / 2) - 1; i >= 0; --i) {
        heapify(begin, i, size);
    }
}

template <class RandomIt>
void popHeap(RandomIt begin, RandomIt end) {
    size_t size = std::distance(begin, end);
    if (size <= 1) return;
    std::iter_swap(begin, end - 1);
    heapify(begin, 0, size - 1);
}
//
//template <class RandomIt>
//void pushHeap(RandomIt begin, RandomIt end) {
//    size_t size = std::distance(begin, end);
//    if (size <= 1) return;
//
//}