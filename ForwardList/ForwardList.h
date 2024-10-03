#pragma once
// Create a custom singly linked list class that would support all the functionality
// given in main.cpp.


#include <stdexcept>
#include <initializer_list> 
#include <iostream>

template <class T>
class ForwardList
{
public:
    ForwardList<T>(std::size_t size=0);
    ForwardList<T>(std::size_t size, T filler);
    bool empty();
    T& front();
private:
    T* m_arr = nullptr;
    std::size_t m_size;
    std::size_t m_capacity;
};


template<class T>
ForwardList<T>::ForwardList<T>(std::size_t size) : m_size(size), m_capacity(size) {
    m_arr = new T[size];
    for (std::size_t i = 0; i < m_size; i++) {
        m_arr[i] = T();
    }
}

template<class T>
ForwardList<T>::ForwardList<T>(std::size_t size, T filler) : m_size(size), m_capacity(size) {
    m_arr = new T[size];
    for (std::size_t i = 0; i < m_size; i++) {
        m_arr[i] = T(filler);
    }
}

template<class T>
bool ForwardList<T>::empty() {
    return m_size == 0;
}

template<class T>
T& ForwardList<T>::front() {
    return m_arr[0];
}

