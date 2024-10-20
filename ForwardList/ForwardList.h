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
    ForwardList<T>(std::size_t size, const T& filler);
    
    bool empty();
    T& front();
    void push_front(const T& value);
    void pop_front();
    bool operator==(const ForwardList<T>& other) const;
    bool operator!=(const ForwardList<T>& other) const;
    ForwardList(const ForwardList<T>& other);

    ~ForwardList() noexcept;
private:
    struct Node {
        T value;
        Node* next;

        Node(): value(T()), next(nullptr) {}
        Node(const T& value, Node* next = nullptr) : value(value), next(next) {}
    };

    Node* head = nullptr;
    std::size_t m_size;

    void clean();
};


template<class T>
ForwardList<T>::ForwardList(std::size_t size) : m_size(size) {
    Node** current = &head;
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node{};
        current = &((*current)->next);
    }

}

template<class T>
ForwardList<T>::ForwardList(std::size_t size, const T& filler) : m_size(size) {
    Node** current = &head;
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node{filler, nullptr };
        //(*current)->value = T(filler);
        current = &((*current)->next);
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

template<class T>
void ForwardList<T>::push_front(const T& value) {
    Node* current = new Node{value, head};
    /*current->value = value;
    current->next = this->head;*/
    head = current;
    m_size++;
}


template<class T>
void ForwardList<T>::pop_front() {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    Node* current = head->next;
    delete head;
    head = current;
    m_size--;
}

template<class T>
void ForwardList<T>::clean() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}


template<class T>
ForwardList<T>::ForwardList(const ForwardList<T>& other) : m_size(other.m_size), head(nullptr) {
    if (!other.head) {
        return;
    }

    head = new Node{ other.head->value, nullptr };
    Node* current = head;
    Node* otherCurrent = other.head->next;

    while (otherCurrent) {
        current->next = new Node{ otherCurrent->value, nullptr };
        current = current->next;
        otherCurrent = otherCurrent->next;
    }
}


template<class T>
bool ForwardList<T>::operator==(const ForwardList<T>& other) const {
    if (m_size != other.m_size) {
        return false;
    }

    Node* current = head;
    Node* otherCurrent = other.head;

    while (current && otherCurrent) {
        if (current->value != otherCurrent->value) {
            return false;
        }
        current = current->next;
        otherCurrent = otherCurrent->next;
    }

    return current == nullptr && otherCurrent == nullptr;
}

template<class T>
bool ForwardList<T>::operator!=(const ForwardList<T>& other) const {
    return !(*this == other);
}

template<class T>
ForwardList<T>::~ForwardList() noexcept {
    clean();
    m_size = 0;
}


