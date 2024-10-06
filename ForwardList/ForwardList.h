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
    struct Node {
        T value;
        Node* next;
    };

    Node* head = nullptr;
    std::size_t m_size;
    std::size_t m_capacity;
};


template<class T>
ForwardList<T>::ForwardList<T>(std::size_t size) : m_size(size), m_capacity(size) {
    Node<T>** current = &head;
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node<T>();
        current = &((*current)->next); // was memory allocated for next???
    }

}

template<class T>
ForwardList<T>::ForwardList<T>(std::size_t size, T filler) : m_size(size), m_capacity(size) {
    Node<T>** current = &head;
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node<T>{};
        (*current)->value = T(filler);
        current = &((*current))->next);
    }
}

template<class T>
bool ForwardList<T>::empty() {
    return m_size == 0;
}

template<class T>
T& ForwardList<T>::front() {
    return head->value;
}

